#pragma once

#include "Projection.h"
#include "Point.h"

using namespace arma;

Projection::Projection(Frame *_frame, vec2 _position, Point *_point)
{
	frame = _frame;
	point = _point;
	position = _position;	

	point->addProjection(this);
}
