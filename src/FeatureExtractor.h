#pragma once

#include <opencv2\opencv.hpp>
#include <armadillo>
#include "Feature.h"

class FeatureExtractor
{
public:
	virtual void extract(cv::Mat image, std::vector<arma::vec2> locations, std::vector<Feature*> &features) = 0;
};