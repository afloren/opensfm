#pragma once

#include <vector>
#include <armadillo>

using namespace arma;
using namespace std;

template <unsigned int K>
bool axis_sort(vec::fixed<K> *a, vec::fixed<K> *b)
{
	return (a->at(0) < b->at(0));
}

template <unsigned int K>
class node
{
public:
	node()
	{
		axis = 0;
		parent = NULL;
		left = NULL;
		right = NULL;
	}

	node(node* _parent, int _axis)
	{
		axis = _axis;
		parent = _parent;
		left = NULL;
		right = NULL;
	}

	~node()
	{

	}

	void insert(vec::fixed<K> *p)
	{
		leaves.push_back(p);
		if(left && right)
		{
			if( p->at(axis) < value )
				left->insert(p);
			else
				right->insert(p);
		}
	}

	void split()
	{		
		sort();
		value = leaves[leaves.size()/2]->at(axis);

		left = new node(this,(axis+1)%K);
		right = new node(this,(axis+1)%K);

		for(int i=0;i<leaves.size();i++)
		{		
			if( leaves[i]->at(axis) < value )
				left->insert(leaves[i]);
			else
				right->insert(leaves[i]);
		}		
	}

	void sort()
	{
		vector<vec::fixed<K>*> L;
		int s, k, p, psize, q, qsize;

		s = leaves.size();
		k = 1;
		for(int k=1;k<s;k*=2)
		{
			p = 0;
			while(p < s)
			{			
				q = min(s,p+k);
				psize = q-p;
				qsize = min(s-q,k);				

				while(psize > 0 || qsize > 0)
				{
					if( qsize == 0 || ( psize > 0 && leaves[p]->at(axis) <= leaves[q]->at(axis) ) )
					{
						L.push_back(leaves[p]);
						p += 1;
						psize -= 1;
					}
					else
					{
						L.push_back(leaves[q]);
						q += 1;
						qsize -= 1;
					}
				}
				p = q;
			}
			leaves = L;
			L.clear();
		}		
	}

	int axis;
	double value;
	node *parent;
	node *left;
	node *right;
	vector<vec::fixed<K>*> leaves;	
};

template <unsigned int K>
class KDTree
{
public:
	KDTree()
	{
		
	}

	~KDTree()
	{

	}

	void addPoint(vec::fixed<K> *p)
	{
		root.insert(p);
	}

	void findNNearest(vec::fixed<K> *p, unsigned int n, vector<vec::fixed<K>*> points)
	{

	}

	void findInRadius(vec::fixed<K> *p, double r, vector<vec::fixed<K>*> points)
	{

	}

	node<K> root;
};