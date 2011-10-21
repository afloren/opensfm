#pragma once
#include <opencv2\opencv.hpp>
#include "Map.h"

class FeatureTracker
{
public:
	virtual void initialize(cv::Mat image, Map *map, double time) = 0;
	virtual void update(cv::Mat image, Map *map, double time) = 0;
};