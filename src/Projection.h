#pragma once

#include <armadillo>

class Frame;
class Point;

class Projection
{
public:
	Projection(Frame *frame, arma::vec2 position, Point *point);
	~Projection();
	
	Frame* frame;
	Point* point;
	arma::vec2 position;
};