#pragma once
#include <vector>
#include <assert.h>

template <typename T, typename K>
class HashTable
{
public:	
	HashTable(int tableSize)
	{
		table.resize(tableSize);
		exists.resize(tableSize,false);
	}

	~HashTable(){}

	T & get(K &key)
	{
		assert(exists[key.hash()]);
		return table[key.hash()];
	}

	void set(K &key, T &val)
	{
		table[key.hash()] = val;
		exists[key.hash()] = true;
	}

	bool has(K &key)
	{
		return exists[key.hash()];
	}

	std::vector<T> table;
	std::vector<bool> exists;
};