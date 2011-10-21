#pragma once

#include "FeatureExtractor.h"
#include "ImagePatch.h"

class ImagePatchExtractor : public FeatureExtractor
{
public:
	void extract(cv::Mat image, std::vector<arma::vec2> locations, std::vector<Feature*> &features);
};