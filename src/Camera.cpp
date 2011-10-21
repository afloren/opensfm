#include "Camera.h"

using namespace arma;

Camera::Camera()
{

}

Camera::Camera(mat m)
{

}

Camera::Camera(double *v)
{

}

Camera::~Camera()
{

}

mat33 Camera::getRotationMatrix()
{
	return mat33();
}

mat34 Camera::getProjectionMatrix()
{
	return mat34();
}

vec3 Camera::getPosition()
{
	return vec3();
}

vec3 Camera::getRotation()
{
	return vec3();
}

vec6 Camera::getParameterVector()
{
	return vec6();
}
