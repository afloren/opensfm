#include "FASTFeatureTracker.h"
#include "ImagePatch.h"
#include "Frame.h"
#include "Point.h"
#include "Projection.h"
#include "Coordinate2DHashTable.h"

#include "FASTFinder.h"
#include "ImagePatchExtractor.h"
#include "ImagePatchMatcher.h"


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

FASTFeatureTracker::FASTFeatureTracker()
{
	featureFinder = new FASTFinder();
	featureExtractor = new ImagePatchExtractor();
	featureMatcher = new ImagePatchMatcher();
}

FASTFeatureTracker::~FASTFeatureTracker()
{
	delete featureFinder;
	delete featureExtractor;
	delete featureMatcher;
}

void FASTFeatureTracker::initialize(cv::Mat image, Map *map, double time)
{
	//cv::Mat_<unsigned char> bw = image;

	std::vector<arma::vec2> locations;
	featureFinder->find(image,locations);

	//findFeatures(bw,keypoints);
	/*
	std::vector<ImagePatch*> patches;
	for(int i=0;i<keypoints.size();i++)
	{
		//extract 11 by 11 patch around keypoint
		std::vector<unsigned char> patch;
		extract_patch(bw,keypoints[i],patch,11,0);
		patches.push_back(new ImagePatch(patch));
	}
	*/
	std::vector<Feature*> features;
	featureExtractor->extract(image,locations,features);

	Frame *frame = map->addFrame(image,time);
	for(int i=0;i<features.size();i++)
	{		
		Point *point = map->addPoint(features[i]);
		frame->addProjection(normalize_projection(Kinv,locations[i]),point);
	}
}

void FASTFeatureTracker::update(cv::Mat image, Map *map, double time)
{
	//cv::Mat_<unsigned char> bw = image;

	//std::vector<arma::vec2> keypoints;
	//findFeatures(bw,keypoints);

	std::vector<arma::vec2> locations;
	featureFinder->find(image,locations);

	/*
	std::vector<ImagePatch*> patches;
	for(int i=0;i<keypoints.size();i++)
	{
		//extract 11 by 11 patch around keypoint
		std::vector<unsigned char> patch;
		extract_patch(bw,keypoints[i],patch,11,0);
		patches.push_back(new ImagePatch(patch));
	}
	*/

	std::vector<Feature*> features;
	featureExtractor->extract(image,locations,features);

	/*
	Coordinate2DHashTable ht(image.rows,image.cols);
	std::vector<ImagePatch*> lastPatches;
	for(int i=0;i<map->frames.back()->projections.size();i++)
	{
		ht.add(i,map->frames.back()->projections[i]->position);
		lastPatches.push_back((ImagePatch*)map->frames.back()->projections[i]->point->feature);
	}

	std::vector<int> matches;
	for(unsigned int i=0;i<keypoints.size();i++)
	{		
		std::vector<unsigned int> indices;
		ht.getInRadius(keypoints[i],30,indices);

		double bestScore = 5000;
		int bestIndex = -1;
		for(unsigned int j=0;j<indices.size();j++)
		{
			double score = ZMSSD(patches[i]->patch,lastPatches[indices[j]]->patch);
			if(score < bestScore)
			{
				bestScore = score;
				bestIndex = indices[j];
			}
		}
		matches.push_back(bestIndex);
	}

	std::vector<std::vector<int>> asdf(map->frames.back()->projections.size());
	for(int i=0;i<matches.size();i++)
	{			
		if(matches[i] >= 0)
		{
			asdf[matches[i]].push_back(i);
		}
	}

	for(int i=0;i<asdf.size();i++)
	{
		double bestScore = 5000;
		int bestIndex = -1;
		for(int j=0;j<asdf[i].size();j++)
		{
			double score = ZMSSD(patches[asdf[i][j]]->patch,lastPatches[i]->patch);
			if(score < bestScore)
			{
				bestScore = score;
				bestIndex = asdf[i][j];
			}
		}
		for(int j=0;j<asdf[i].size();j++)
		{
			if(asdf[i][j] != bestIndex)
			{
				matches[asdf[i][j]] = -1;
			}
		}
	}
	*/
	std::vector<Feature*> lastFeatures;
	for(int i=0;i<map->frames.back()->points.size();i++)
	{
		lastFeatures.push_back(map->frames.back()->points[i]->feature);
	}

	std::vector<int> matches;
	featureMatcher->match(features,lastFeatures,matches);

	Frame *lastFrame = map->frames.back();
	Frame *frame = map->addFrame(image,time);
	for(int i=0;i<features.size();i++)
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

void FASTFeatureTracker::findFeatures(cv::Mat image, std::vector<arma::vec2> &features)
{
	std::vector<cv::KeyPoint> keypoints;
	cv::FAST(image,keypoints,20,true);

	for(int i=0;i<keypoints.size();i++)
	{
		arma::vec2 p;
		p[0] = keypoints[i].pt.x;
		p[1] = keypoints[i].pt.y;
		features.push_back(p);
	}
}
