#include "ImagePatchExtractor.h"

template <typename T>
void extract_patch(cv::Mat_<T> image, arma::vec2 point, std::vector<T> &patch, int bsize=11, int padding=7)
{
	patch.clear();
	patch.reserve(bsize*bsize+padding);

	int s = (bsize-1)/2;//bsize should be odd

	for(int j = -s; j <= s; j++)
	{
		for( int k = -s; k <= s; k++)
		{
			int c = point[0] + j;
			int r = point[1] + k;

			if(c < 0 || r < 0 || c >= image.cols || r >= image.rows)
			{
				patch.push_back(0);
			}
			else
			{
				patch.push_back(image(r,c));
			}
		}
	}
	for(int j=0;j<padding;j++)
	{
		patch.push_back(0);
	}
}

void ImagePatchExtractor::extract(cv::Mat image, std::vector<arma::vec2> locations, std::vector<Feature*> &features)
{
	for(int i=0;i<locations.size();i++)
	{
		//extract 11 by 11 patch around keypoint
		std::vector<unsigned char> patch;
		extract_patch<unsigned char>(image,locations[i],patch,11,0);
		features.push_back(new ImagePatch(image,patch,locations[i]));
	}
}