#include "Coordinate2DHashTable.h"

Coordinate2DHashKey::Coordinate2DHashKey(arma::vec::fixed<2> &_vec, int _rows, int _cols)
{
	vec = _vec;
	rows = _rows;
	cols = _cols;
}

Coordinate2DHashKey::~Coordinate2DHashKey()
{

}

unsigned int Coordinate2DHashKey::hash()
{
	return floor(vec[0]+vec[1]*cols+0.5);
}


Coordinate2DHashTable::Coordinate2DHashTable(int _rows, int _cols)
	: HashTable(_rows*_cols)
{
	rows = _rows;
	cols = _cols;
}

Coordinate2DHashTable::~Coordinate2DHashTable()
{

}

void Coordinate2DHashTable::add(unsigned int index, arma::vec::fixed<2> &coordinate)
{
	set(Coordinate2DHashKey(coordinate,rows,cols),index);
}

bool Coordinate2DHashTable::has(arma::vec::fixed<2> &coordinate)
{
	return HashTable::has(Coordinate2DHashKey(coordinate,rows,cols));
}

unsigned int Coordinate2DHashTable::get(arma::vec::fixed<2> &coordinate)
{
	
	return HashTable::get(Coordinate2DHashKey(coordinate,rows,cols));
}

void Coordinate2DHashTable::getInRadius(arma::vec::fixed<2> &coordinate, double radius, std::vector<unsigned int> &indices)
{
	//use l1 distance to cheat
	int w = floor(radius);	
	for(int i = -w; i < w; i++)
	{
		for(int j = -w; j < w; j++)
		{
			if( abs(i) + abs(j) < w )
			{
				arma::vec::fixed<2> c;
				c[0] = coordinate[0] + i;
				c[1] = coordinate[1] + j;
				if(c[0] >= 0 && c[0] < cols && c[1] >= 0 && c[1] < rows)
				{
					if(has(c)) 
					{
						indices.push_back(get(c));
					}
				}
			}			
		}
	}
}

void Coordinate2DHashTable::getInPatch(arma::vec::fixed<2> &coordinate, double size, std::vector<unsigned int> &indices)
{
	int w = floor(size/2);
	arma::vec::fixed<2> c;
	for(int i = -w; i < w; i++)
	{
		for(int j = -w; j < w; j++)
		{
			arma::vec::fixed<2> c;
			c[0] = coordinate[0] + i;
			c[1] = coordinate[1] + j;
			if(c[0] >= 0 && c[0] < cols && c[1] >= 0 && c[1] < rows)
			{
				if(has(c))
				{
					indices.push_back(get(c));
				}
			}
		}
	}
}

