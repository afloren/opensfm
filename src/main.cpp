#include <stdio.h>
#include <string>
#include <opencv2\opencv.hpp>
#include <sba.h>
#include <armadillo>

#include "FASTFeatureTracker.h"
#include "Timer.h"
#include "Map.h"
#include "Frame.h"
#include "Camera.h"
#include "Point.h"
#include "Projection.h"

#ifdef _DEBUG
#pragma comment(lib, "opencv_core231d.lib")
#pragma comment(lib, "opencv_highgui231d.lib")
#pragma comment(lib, "opencv_features2d231d.lib")
#pragma comment(lib, "opencv_imgproc231d.lib")
#else
#pragma comment(lib, "opencv_core231.lib")
#pragma comment(lib, "opencv_highgui231.lib")
#pragma comment(lib, "opencv_features2d231.lib")
#pragma comment(lib, "opencv_imgproc231.lib")
#endif

int main(int argc, char *argv[])
{		
	cv::VideoCapture cap(0);

	Map map;
	FASTFeatureTracker ft;
	ft.K = arma::eye(3,3);
	ft.Kinv = arma::inv(ft.K);

	Timer timer;
	timer.start();

	cv::Mat image, image0, image1, image2, image3;
	cap >> image;

	image0 = image.clone();
	cv::pyrDown(image0,image1);
	cv::pyrDown(image1,image2);
	cv::pyrDown(image2,image3);

	cv::Mat_<unsigned char> bw, bw0, bw1, bw2, bw3;
	cv::cvtColor(image,bw,CV_RGB2GRAY);
	cv::cvtColor(image0,bw0,CV_RGB2GRAY);
	cv::cvtColor(image1,bw1,CV_RGB2GRAY);
	cv::cvtColor(image2,bw2,CV_RGB2GRAY);
	cv::cvtColor(image3,bw3,CV_RGB2GRAY);

	ft.initialize(bw1,&map,timer.getTotal());

	for(;;)
	{		
		timer.getElapsed();

		cap >> image;
		image0 = image.clone();
		cv::pyrDown(image0,image1);
		cv::pyrDown(image1,image2);
		cv::pyrDown(image2,image3);
		
		cv::cvtColor(image,bw,CV_RGB2GRAY);
		cv::cvtColor(image0,bw0,CV_RGB2GRAY);
		cv::cvtColor(image1,bw1,CV_RGB2GRAY);
		cv::cvtColor(image2,bw2,CV_RGB2GRAY);
		cv::cvtColor(image3,bw3,CV_RGB2GRAY);

		ft.update(bw1,&map,timer.getTotal());

		for(int i=0;i<map.frames.back()->projections.size();i++)
		{
			Point *point = map.frames.back()->projections[i]->point;
			if(point->projections.size() > 1)
			{
				cv::Point2f p1;
				p1.x = point->projections[point->projections.size()-2]->position[0];
				p1.y = point->projections[point->projections.size()-2]->position[1];

				cv::Point2f p2;
				p2.x = point->projections[point->projections.size()-1]->position[0];
				p2.y = point->projections[point->projections.size()-1]->position[1];

				cv::line(image1,p1,p2,cv::Scalar(255,0,0),2);
			}
		}

		cv::imshow("Image",image1);

		//cv::imshow("Image",image2);
		//cv::imshow("Level 0",image0);
		//cv::imshow("Level 1",image1);
		//cv::imshow("Level 2",image2);
		//cv::imshow("Level 3",image3);

		if(cv::waitKey(30) >= 0)
		{
			break;			
		}
	}

	return 0;
}