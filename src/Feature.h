#pragma once

#include <opencv2\opencv.hpp>
#include <armadillo>

class Feature
{
public:	
	Feature(cv::Mat _image, arma::vec2 _position)
	{
		image = _image;
		position = _position;
	}

	virtual double distance(Feature *f) = 0;

	cv::Mat image;
	arma::vec2 position;
};