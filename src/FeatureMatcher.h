#pragma once

#include <vector>
#include "Feature.h"

class FeatureMatcher
{
public:
	virtual void match(std::vector<Feature*> features1, std::vector<Feature*> features2, std::vector<int> &matches) = 0;
};