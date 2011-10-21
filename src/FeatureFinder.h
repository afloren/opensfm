#pragma once

#include <opencv2\opencv.hpp>
#include <armadillo>

class FeatureFinder
{
public:
	virtual void find(cv::Mat image, std::vector<arma::vec2> &locations) = 0;
};