#pragma once

#include <opencv2\opencv.hpp>
#include "Map.h"
#include "FeatureFinder.h"
#include "FeatureExtractor.h"
#include "FeatureMatcher.h"

class FeatureTracker
{
public:
	FeatureTracker();
	~FeatureTracker();

	void initialize(cv::Mat image, Map *map, double time);
	void update(cv::Mat image, Map *map, double time);

	FeatureFinder *featureFinder;
	FeatureExtractor *featureExtractor;
	FeatureMatcher *featureMatcher;

	arma::mat33 K;
	arma::mat33 Kinv;
};