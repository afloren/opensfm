#pragma once

#include <opencv2\opencv.hpp>
#include <armadillo>


class Frame;
class Point;
class Feature;

class Map
{
public:
	Map();
	~Map();

	Frame* addFrame(cv::Mat image, double time);
	Point* addPoint(Feature* feature = NULL);

	std::vector<Frame*> frames;
	std::vector<Point*> points;
};