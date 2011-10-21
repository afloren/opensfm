#pragma once

#include <opencv2\opencv.hpp>
#include <armadillo>

class Map;
class Camera;
class Projection;
class Point;

class Frame
{
public:
	Frame(Map *map, cv::Mat image, double time);
	~Frame();

	void addProjection(arma::vec2 projection);
	void addProjection(arma::vec2 projection, Point *point);

	Map *map;
	cv::Mat image;
	double time;
	Camera *camera;
	std::vector<Projection*> projections;
	std::vector<Point*> points;
};