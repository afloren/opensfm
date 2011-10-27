#include "ImagePatch.h"

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

ImagePatch::ImagePatch(cv::Mat _image, arma::vec2 _position, std::vector<unsigned char> _patch) : Feature(_image,_position)
{
	patch = _patch;
}

ImagePatch::~ImagePatch()
{

}

double ImagePatch::distance(Feature *f)
{
	ImagePatch *p = static_cast<ImagePatch*>(f);
	return ZMSSD(patch,p->patch);
}