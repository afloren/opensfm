#include "Point.h"
#include "Feature.h"

using namespace arma;

Point::Point(Map *_map, Feature *_feature)
{
	map = _map;	
	feature = _feature;
}

Point::Point(Map *_map, vec3 _position, Feature *_feature)
{
	map = _map;
	position = _position;
	feature = _feature;
}

void Point::addProjection(Projection *projection)
{
	projections.push_back(projection);
}
