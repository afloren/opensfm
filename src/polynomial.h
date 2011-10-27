#pragma once

#include <armadillo>

double evaluate_polynomial( arma::vec &c, double val );
void solve_polynomial( arma::vec &c, arma::vec &r, double min, double max, int max_iter, double tol );