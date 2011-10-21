#include "Frame.h"
#include "Map.h"
#include "Projection.h"

using namespace arma;

Frame::Frame(Map *_map, cv::Mat _image, double _time)
{
	map = _map;
	image = _image.clone();
	time = _time;
}

Frame::~Frame()
{

}

void Frame::addProjection(vec2 projection)
{
	Point *newPoint = map->addPoint();
	Projection *newProjection = new Projection(this,projection,newPoint);
	projections.push_back(newProjection);
	points.push_back(newPoint);
}

void Frame::addProjection(vec2 projection, Point *point)
{
	Projection *newProjection = new Projection(this,projection,point);
	projections.push_back(newProjection);
	points.push_back(point);
}