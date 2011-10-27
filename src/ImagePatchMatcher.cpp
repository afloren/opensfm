#include "ImagePatchMatcher.h"
#include "Coordinate2DHashTable.h"
#include "Frame.h"
#include "Projection.h"
#include "Point.h"

void ImagePatchMatcher::match(std::vector<Feature*> features1, std::vector<Feature*> features2, std::vector<int> &matches)
{
	int rows = 0, cols = 0;

	if( features2.size() > 0 )
	{
		rows = features2[0]->image.rows;
		cols = features2[0]->image.cols;
	}

	Coordinate2DHashTable ht(rows,cols);

	for(int i=0;i<features2.size();i++)
	{
		ht.add(i,features2[i]->position);
	}	

	for(int i=0;i<features1.size();i++)
	{	
		std::vector<unsigned int> indices;
		ht.getInRadius(features1[i]->position,30,indices);

		double bestScore = 5000;
		int bestIndex = -1;
		for(unsigned int j=0;j<indices.size();j++)
		{
			double score = features1[i]->distance(features2[indices[j]]);
			if(score < bestScore)
			{
				bestScore = score;
				bestIndex = indices[j];
			}
		}
		matches.push_back(bestIndex);
	}

	std::vector<std::vector<int>> asdf(features2.size());
	for(int i=0;i<matches.size();i++)
	{			
		if(matches[i] >= 0)
		{
			asdf[matches[i]].push_back(i);
		}
	}

	for(int i=0;i<asdf.size();i++)
	{
		double bestScore = 5000;
		int bestIndex = -1;
		for(int j=0;j<asdf[i].size();j++)
		{
			double score = features1[asdf[i][j]]->distance(features2[i]);
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