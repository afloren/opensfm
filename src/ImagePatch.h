#pragma once

#include <vector>
#include <opencv2\opencv.hpp>
#include <armadillo>
#include "Feature.h"

class ImagePatch : public Feature
{
public:
	ImagePatch()
	{

	}

	ImagePatch(cv::Mat _image, std::vector<unsigned char> _patch, arma::vec2 _location)
	{
		image = _image;
		patch = _patch;
		location = _location;
	}

	~ImagePatch()
	{

	}

	cv::Mat image;
	std::vector<unsigned char> patch;
	arma::vec2 location;
};