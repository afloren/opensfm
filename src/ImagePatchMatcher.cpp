#include "ImagePatchMatcher.h"
#include "Coordinate2DHashTable.h"

template <typename T>
double ZMSSD(std::vector<T> &patch1, std::vector<T> &patch2)
{
	assert(patch1.size() == patch2.size());

	int n = patch1.size();
	double sum1 = 0, sum2 = 0, zmssd = 0;

	for(int i=0;i<n;i++)
	{
		sum1 += patch1[i];
		sum2 += patch2[i];		
	}	
	for(int i=0;i<n;i++)
	{
		zmssd += pow(patch1[i]-sum1/n-patch2[i]+sum2/n,2);
	}

	return zmssd;
}

void ImagePatchMatcher::match(std::vector<Feature*> features1, std::vector<Feature*> features2, std::vector<int> &matches)
{
	std::vector<ImagePatch*> imagePatches1(features1.size()), imagePatches2(features2.size());
	for(int i=0;i<features1.size();i++)
	{
		imagePatches1[i] = static_cast<ImagePatch*>(features1[i]);
	}
	for(int i=0;i<features2.size();i++)
	{
		imagePatches2[i] = static_cast<ImagePatch*>(features2[i]);
	}
	match(imagePatches1,imagePatches2,matches);
}

void ImagePatchMatcher::match(std::vector<ImagePatch*> imagePatches1, std::vector<ImagePatch*> imagePatches2, std::vector<int> &matches)
{
	if(imagePatches1.size() > 0 && imagePatches2.size() > 0)
	{
		assert(imagePatches1[0]->image.rows == imagePatches2[0]->image.rows);
		assert(imagePatches1[0]->image.cols == imagePatches2[0]->image.cols);
	}

	Coordinate2DHashTable ht(imagePatches1[0]->image.rows,imagePatches1[0]->image.cols);

	for(int i=0;i<imagePatches2.size();i++)
	{
		ht.add(i,imagePatches2[i]->location);
	}	

	for(int i=0;i<imagePatches1.size();i++)
	{	
		std::vector<unsigned int> indices;
		ht.getInRadius(imagePatches1[i]->location,30,indices);

		double bestScore = 5000;
		int bestIndex = -1;
		for(unsigned int j=0;j<indices.size();j++)
		{
			double score = ZMSSD(imagePatches1[i]->patch,imagePatches2[indices[j]]->patch);
			if(score < bestScore)
			{
				bestScore = score;
				bestIndex = indices[j];
			}
		}
		matches.push_back(bestIndex);
	}

	std::vector<std::vector<int>> asdf(imagePatches2.size());
	for(int i=0;i<matches.size();i++)
	{			
		if(matches[i] >= 0)
		{
			asdf[matches[i]].push_back(i);
		}
	}

	for(int i=0;i<asdf.size();i++)
	{
		//ImagePatch *patch2 = (ImagePatch*)features2[i];
		double bestScore = 5000;
		int bestIndex = -1;
		for(int j=0;j<asdf[i].size();j++)
		{
			//ImagePatch *patch1 = (ImagePatch*)features1[asdf[i][j]];
			double score = ZMSSD(imagePatches1[asdf[i][j]]->patch,imagePatches2[i]->patch);
			if(score < bestScore)
			{
				bestScore = score;
				bestIndex = asdf[i][j];
			}
		}
		for(int j=0;j<asdf[i].size();j++)
		{
			if(asdf[i][j] != bestIndex)
			{
				matches[asdf[i][j]] = -1;
			}
		}
	}
}