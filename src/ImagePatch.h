#pragma once

#include <vector>
#include <opencv2\opencv.hpp>
#include <armadillo>
#include "Feature.h"

class ImagePatch : public Feature
{
public:	
	ImagePatch(cv::Mat image, arma::vec2 position, std::vector<unsigned char> patch);
	~ImagePatch();

	double distance(Feature *f);

	std::vector<unsigned char> patch;
};