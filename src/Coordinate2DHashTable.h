#pragma once
#include <armadillo>
#include "HashTable.h"

class Coordinate2DHashKey
{
public:
	Coordinate2DHashKey(arma::vec::fixed<2> &vec, int rows, int cols);
	~Coordinate2DHashKey();

	unsigned int hash();

	arma::vec::fixed<2> vec;
	int rows, cols;
};

class Coordinate2DHashTable : public HashTable<unsigned int, Coordinate2DHashKey>
{
public:
	Coordinate2DHashTable(int rows, int cols);
	~Coordinate2DHashTable();

	void add(unsigned int index, arma::vec::fixed<2> &coordinate);	
	bool has(arma::vec::fixed<2> &coordinate);
	unsigned int get(arma::vec::fixed<2> &coordinate);	
	void getInRadius(arma::vec::fixed<2> &coordinate, double radius, std::vector<unsigned int> &indices);
	void getInPatch(arma::vec::fixed<2> &coordinate, double size, std::vector<unsigned int> &indices);

	int rows, cols;
};