#pragma once

#include "FeatureMatcher.h"
#include "ImagePatch.h"

class ImagePatchMatcher : public FeatureMatcher
{
public:
	void match(std::vector<Feature*> features1, std::vector<Feature*> features2, std::vector<int> &matches);
	void match(std::vector<ImagePatch*> imagePatches1, std::vector<ImagePatch*> imagePatches2, std::vector<int> &matches);
	
};