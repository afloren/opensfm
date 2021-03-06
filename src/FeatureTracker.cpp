#include "FeatureTracker.h"
#include "Frame.h"
#include "Point.h"
#include "Projection.h"

arma::vec2 normalize_projection(arma::mat33 Kinv, arma::vec2 projection)
{
	arma::vec3 p;
	p[0] = projection[0];
	p[1] = projection[1];
	p[2] = 1.0;
	
	p = Kinv*p;

	arma::vec2 r;
	r[0] = p[0];
	r[1] = p[1];

	return r;
}

template <typename T>
arma::Mat<T> cv2arma(cv::Mat_<T> m)
{
	arma::mat r(m.rows,m.cols);
	for(int i=0;i<m.rows;i++)
	{
		for(int j=0;j<m.cols;j++)
		{
			r(i,j) = m(i,j);
		}
	}
	return r;
}

FeatureTracker::FeatureTracker(
	FeatureFinder *_featureFinder, 
	FeatureExtractor *_featureExtractor, 
	FeatureMatcher *_featureMatcher,
	arma::mat33 _K)
{
	featureFinder = _featureFinder;
	featureExtractor = _featureExtractor;
	featureMatcher = _featureMatcher;
	K = _K;
	Kinv = arma::inv(K);
}

FeatureTracker::~FeatureTracker()
{

}

void FeatureTracker::initialize(cv::Mat image, Map *map, double time)
{
	std::vector<arma::vec2> locations;
	featureFinder->find(image,locations);

	std::vector<Feature*> features;
	featureExtractor->extract(image,locations,features);

	Frame *frame = map->addFrame(image,time);
	for(unsigned int i=0;i<features.size();i++)
	{		
		Point *point = map->addPoint(features[i]);
		frame->addProjection(normalize_projection(Kinv,locations[i]),point);
	}
}

void FeatureTracker::update(cv::Mat image, Map *map, double time)
{
	std::vector<arma::vec2> locations;
	featureFinder->find(image,locations);
	
	std::vector<Feature*> features;
	featureExtractor->extract(image,locations,features);

	std::vector<Feature*> lastFeatures;
	for(unsigned int i=0;i<map->frames.back()->points.size();i++)
	{
		lastFeatures.push_back(map->frames.back()->points[i]->feature);
	}

	std::vector<int> matches;
	featureMatcher->match(features,lastFeatures,matches);

	Frame *lastFrame = map->frames.back();
	Frame *frame = map->addFrame(image,time);
	for(unsigned int i=0;i<features.size();i++)
	{
		if(matches[i] >= 0)
		{
			Point *point = lastFrame->points[matches[i]];
			point->feature = features[i];//TODO: this is a memory leak
			frame->addProjection(normalize_projection(Kinv,locations[i]),point);
		}
		else
		{
			Point *point = map->addPoint(features[i]);
			frame->addProjection(normalize_projection(Kinv,locations[i]),point);
		}
	}
}

