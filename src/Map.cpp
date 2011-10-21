#include "Map.h"

#include "Frame.h"
#include "Point.h"
#include "Feature.h"

Map::Map()
{

}

Map::~Map()
{

}

Frame* Map::addFrame(cv::Mat image, double time)
{
	Frame *newFrame = new Frame(this,image,time);
	frames.push_back(newFrame);
	return newFrame;
}

Point* Map::addPoint(Feature *feature)
{
	Point *newPoint = new Point(this,feature);
	points.push_back(newPoint);
	return newPoint;
}