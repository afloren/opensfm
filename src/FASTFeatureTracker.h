#pragma once

#include "FeatureTracker.h"
#include "FeatureFinder.h"
#include "FeatureExtractor.h"
#include "FeatureMatcher.h"

class FASTFeatureTracker : public FeatureTracker
{
public:
	FASTFeatureTracker();
	~FASTFeatureTracker();

	void initialize(cv::Mat image, Map *map, double time);
	void update(cv::Mat image, Map *map, double time);

	void findFeatures(cv::Mat image, std::vector<arma::vec2> &locations);
	//void extractFeatures(cv::Mat image, std::vector<arma::vec2> &locations, std::vector<ImagePatch*> patches);
	//void findFeatureMatches(cv::Mat image, std::vector<ImagePatch*> features, std::vector<ImagePatch*> features2, std::vector<int> matches);

	FeatureFinder *featureFinder;
	FeatureExtractor *featureExtractor;
	FeatureMatcher *featureMatcher;

	arma::mat33 K;
	arma::mat33 Kinv;
};