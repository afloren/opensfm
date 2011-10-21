#pragma once

#include <armadillo>
#include "Feature.h"

class Map;
class Projection;

class Point
{
public:
	Point(Map *map, Feature *feature = NULL);
	Point(Map *map, arma::vec3 position, Feature *feature = NULL);
	~Point();
	
	void addProjection(Projection* projection);

	Map *map;
	Feature *feature;
	std::vector<Projection*> projections;
	arma::vec3 position;
};