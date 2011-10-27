#include "polynomial.h"

using namespace arma;

void shift_left( vec &a, unsigned int n )
{
	for(unsigned int i=0;i<n;i++)
	{
		for(unsigned int j=0;j<a.size()-1;j++)
		{
			a(j) = a(j+1);
		}
		a(a.size()-1) = 0.0;
	}
}

void shift_right( vec &a, unsigned int n )
{
	for(unsigned int i=0;i<n;i++)
	{
		for(unsigned int j=a.size()-1;j>0;j--)
		{
			a(j) = a(j-1);
		}
		a(0) = 0.0;
	}
}

void polynomial_derivative( vec &c )
{
	shift_right(c,1);
	for(int i=0;i<c.size();i++)
	{
		c(i) *= (c.size()-i);
	}
}

double evaluate_polynomial( vec &c, double val )
{
	double y = c(c.size()-1);
	double x = val;
	for(int i=c.size()-2;i>=0;i--)
	{
		y += c(i)*x;
		x *= val;
	}
	return y;
}

int first_nonzero( vec &a )
{
	int fnz = a.size()-1;
	for(int i=0;i<a.size();i++)
	{
		if( a(i) != 0.0 )
		{
			fnz = i;
			break;
		}
	}
	return fnz;
}

int degree( vec &c )
{
	return (c.size() - first_nonzero(c) - 1);
}

void polynomial_division( vec &a, vec &b, vec &q, vec &r )
{
	q = b;//TODO: hopefully this copies data
	shift_left(q,degree(a)-degree(b));

	q *= a(first_nonzero(a))/b(first_nonzero(b));

	r = a - q;

	r(first_nonzero(q)) = 0.0;//only necessary due to rounding errors

	if( degree(r) >= degree(b) )
	{
		vec qp;
		polynomial_division(r,b,qp,r);
		q = q + qp;
	}
}

void sturm_chain( std::vector<vec> &chain, vec &r0, vec &r1 )
{
	vec q, r2;
	polynomial_division(r0,r1,q,r2);
	r2 = -r2;

	chain.push_back(r2);

	if( degree(r2) > 0 )
	{
		sturm_chain(chain,r1,r2);
	}
}

void construct_sturm_chain( std::vector<vec> &chain, vec &c )
{
	vec d = c;//TODO: hope this copies data
	polynomial_derivative(d);
	chain.push_back(c);
	chain.push_back(d);
	sturm_chain(chain,c,d);
}

int evaluate_sturm_chain( std::vector<vec> &chain, double val )
{
	int sc = 0;

	double x1 = 0;
	for(unsigned int i=0;i<chain.size();i++)
	{
		double x2 = evaluate_polynomial(chain[i],val);
		if( (x1*x2) < 0 )
		{
			sc++;
		}
		x1 = x2;
	}

	return sc;
}

void bracket_roots( std::vector<vec2> &brackets, std::vector<vec> &chain, double min, double max, double tol )
{	
	double t = (max-min)/2.0;

	int num_zeros = evaluate_sturm_chain(chain,min) - evaluate_sturm_chain(chain,max);

	if( num_zeros == 1 || (t < tol && num_zeros > 0 ) )//TODO: sort of a hack for when multiple zeros are within tol of each other
	{
		vec2 bracket;
		bracket(0) = min;
		bracket(1) = max;
		brackets.push_back(bracket);
	}
	else if( num_zeros > 1 )
	{
		double mid = min + t;
		bracket_roots(brackets,chain,min,mid,tol);
		bracket_roots(brackets,chain,mid,max,tol);
	}
}

double polish_root( vec &p, double min, double max, int max_iter, double tol )
{
	double c = 0.0;

	for(int n=0;n<max_iter;n++)
	{
		c = (min+max)/2.0;
		double cval = evaluate_polynomial(p,c);
		if( (max-min)/2.0 < tol || cval == 0.0 )
		{
			break;
		}

		if( (evaluate_polynomial(p,min)*cval) < 0 )
		{
			max = c;
		}
		else
		{
			min = c;
		}
	}

	return c;
}


void solve_polynomial(vec &c, vec &r, double min, double max, int max_iter, double tol)
{
	std::vector<vec> chain;
	construct_sturm_chain(chain,c);

	std::vector<vec2> brackets;
	bracket_roots(brackets,chain,min,max,tol);//TODO: this tol shouldn't necessarily be the same as the polish tol
	//TODO: evaluate total number of roots and grow min/max until all are included

	r = vec(brackets.size());
	for(unsigned int i=0;i<brackets.size();i++)
	{
		r(i) = polish_root(c,brackets[i](0),brackets[i](1),max_iter,tol);
	}
}
