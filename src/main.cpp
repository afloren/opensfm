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

	//std::vector<cv::KeyPoint> keypoints, lastKeypoints;
	//std::vector<std::vector<unsigned char>> patches, lastPatches;
	//std::vector<std::vector<double>> distances;
	//std::vector<cv::KeyPoint> keypoints0, keypoints1, keypoints2, keypoints3;
	/*
	cv::Mat descriptors, lastDescriptors;
	std::vector<cv::DMatch> matches;
	std::vector<cv::DMatch> matches2;
	*/
	//cv::FeatureDetector *fd = new cv::FastFeatureDetector(10,true);
	//cv::DescriptorExtractor *de = new cv::SiftDescriptorExtractor();
	//cv::DescriptorMatcher *dm = new cv::BruteForceMatcher<cv::L2<float>>();

	//fd->detect(bw,lastKeypoints);	
	//keypoints.resize(MIN(lastKeypoints.size(),200));
	//de->compute(bw,keypoints,lastDescriptors);			

	for(;;)
	{		
		timer.getElapsed();

		cap >> image;
		image0 = image.clone();
		cv::pyrDown(image0,image1);
		cv::pyrDown(image1,image2);
		cv::pyrDown(image2,image3);

		//Frame *currentFrame = map.createFrame(image,timer.getTotal());
		
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

		/*
		std::cout << "image load time: " << timer.getElapsed() << "\n";

		try
		{			
			cv::FAST(bw2,keypoints,20,true);
			//cv::FAST(bw0,keypoints0,20,false);
			//cv::FAST(bw1,keypoints1,20,false);
			//cv::FAST(bw2,keypoints2,20,false);
			//cv::FAST(bw3,keypoints3,20,false);			
		}
		catch(cv::Exception e)
		{
			std::cout << e.what();
		}

		std::cout << "FAST feature extraction: " << timer.getElapsed() << "\n";
		
		patches.clear();
		for(int i=0;i<keypoints.size();i++)
		{
			//extract 11 by 11 patch around keypoint
			std::vector<unsigned char> patch;
			extract_patch(bw2,keypoints[i].pt,patch,11,0);
			patches.push_back(patch);
		}

		std::cout << "patch extraction: " << timer.getElapsed() << "\n";		

		Coordinate2DHashTable ht(bw2.rows,bw2.cols);

		for(unsigned int i=0;i<lastKeypoints.size();i++)
		{
			arma::vec::fixed<2> coord;
			coord[0] = lastKeypoints[i].pt.x;
			coord[1] = lastKeypoints[i].pt.y;
			ht.add(i,coord);
		}

		std::cout << "Build HashTable: " << timer.getElapsed() << "\n";

		std::vector<int> matches;
		for(unsigned int i=0;i<keypoints.size();i++)
		{
			arma::vec::fixed<2> coord;
			coord[0] = keypoints[i].pt.x;
			coord[1] = keypoints[i].pt.y;
			std::vector<unsigned int> indices;
			ht.getInRadius(coord,30,indices);

			double bestScore = 5000;
			int bestIndex = -1;
			for(unsigned int j=0;j<indices.size();j++)
			{
				double score = ZMSSD(patches[i],lastPatches[indices[j]]);
				if(score < bestScore)
				{
					bestScore = score;
					bestIndex = indices[j];
				}
			}
			matches.push_back(bestIndex);
		}

		std::vector<std::vector<int>> asdf(lastKeypoints.size());
		for(int i=0;i<matches.size();i++)
		{			
			if(matches[i] >= 0)
			{
				asdf[matches[i]].push_back(i);
			}
		}

		for(int i=0;i<asdf.size();i++)
		{
			double bestScore = 2000000;
			int bestIndex = -1;
			for(int j=0;j<asdf[i].size();j++)
			{
				double score = ZMSSD(patches[asdf[i][j]],lastPatches[i]);
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

		std::cout << "Find matches in radius: " << timer.getElapsed();			

		for(int i=0;i<keypoints.size();i++)
		{
			//arma::vec::fixed<2> coord;
			//coord[0] = keypoints[keypoints.size()/2].pt.x;
			//coord[1] = keypoints[keypoints.size()/2].pt.y;
			//std::vector<unsigned int> indices;
			//ht.getInRadius(coord,40,indices);

			//cv::line(image2,keypoints[i].pt,keypoints[i].pt,cv::Scalar(255,0,0),4);			
			//cv::line(image2,keypoints[keypoints.size()/2].pt,keypoints[keypoints.size()/2].pt,cv::Scalar(0,255,0),3);
			//for(int i=0;i<indices.size();i++)
			//{
				//cv::line(image2,lastKeypoints[indices[i]].pt,lastKeypoints[indices[i]].pt,cv::Scalar(0,0,255),2);
			//}
			
			if(matches[i] >= 0)
			{
				cv::line(image2,keypoints[i].pt,lastKeypoints[matches[i]].pt,cv::Scalar(0,0,255),2);
			}			
		}
		*/
		/*
		for(int i=0;i<keypoints0.size();i++)
		{
			cv::line(image0,keypoints0[i].pt,keypoints0[i].pt,cv::Scalar(255,0,0),2);
		}
		for(int i=0;i<keypoints1.size();i++)
		{
			cv::line(image1,keypoints1[i].pt,keypoints1[i].pt,cv::Scalar(255,0,0),2);
		}
		for(int i=0;i<keypoints2.size();i++)
		{
			cv::line(image2,keypoints2[i].pt,keypoints2[i].pt,cv::Scalar(255,0,0),2);
		}
		for(int i=0;i<keypoints3.size();i++)
		{
			cv::line(image3,keypoints3[i].pt,keypoints3[i].pt,cv::Scalar(255,0,0),2);
		}
		*/

		//lastKeypoints = keypoints;
		//lastPatches = patches;
		//lastDescriptors = descriptors;

		//arma::vec2 test;
		//test << 1.0 << 2.0;

		//currentFrame->addProjection(test);

		//cv::imshow("Image",image2);
		//cv::imshow("Level 0",image0);
		//cv::imshow("Level 1",image1);
		//cv::imshow("Level 2",image2);
		//cv::imshow("Level 3",image3);

		//lastKeypoints = keypoints;
		//lastPatches = patches;

		if(cv::waitKey(30) >= 0)
		{
			break;			
		}
	}

	return 0;
}