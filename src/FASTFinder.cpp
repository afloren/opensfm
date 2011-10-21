#include "FASTFinder.h"

void FASTFinder::find(cv::Mat image, std::vector<arma::vec2> &locations)
{
	std::vector<cv::KeyPoint> keypoints;
	cv::FAST(image,keypoints,20,true);

	for(int i=0;i<keypoints.size();i++)
	{
		arma::vec2 p;
		p[0] = keypoints[i].pt.x;
		p[1] = keypoints[i].pt.y;
		locations.push_back(p);
	}
}