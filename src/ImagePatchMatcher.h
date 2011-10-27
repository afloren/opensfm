#pragma once

#include "FeatureMatcher.h"

class ImagePatchMatcher : public FeatureMatcher
{
public:
	void match(std::vector<Feature*> features1, std::vector<Feature*> features2, std::vector<int> &matches);	
};