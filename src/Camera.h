#pragma once

#include <armadillo>

namespace arma
{
	typedef arma::Mat<double>::fixed<3U,4U> mat34;
}

class Camera
{
public:
	Camera();
	Camera(arma::mat m);
	Camera(double *v);
	~Camera();

	arma::mat33 getRotationMatrix();
	arma::mat34 getProjectionMatrix();
	arma::vec3 getPosition();
	arma::vec3 getRotation();
	arma::vec6 getParameterVector();

protected:
	double v[6];
};