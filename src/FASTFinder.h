#pragma once

#include "FeatureFinder.h"

class FASTFinder : public FeatureFinder
{
public:

	void find(cv::Mat image, std::vector<arma::vec2> &locations);
};