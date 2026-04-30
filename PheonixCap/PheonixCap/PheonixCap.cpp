// PheonixCap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Grabber.h"
#include<time.h>
#include <windows.h>


int main()
{
	clock_t start, end;
	_mkdir("D:\\Bhabhatron_gigE_calib");
	const char ESC_KEY = 27;
	int i = 0;
	Grabber gigE;
	gigE.show_camera_Props();
	cv::Mat imgOrig;
	cv::Mat imgOrig1;
	if (gigE.isGigE_initialized()) {
		while (1)
		{
			gigE.grabFrame(imgOrig);

			//cv::imshow("Frame_", imgOrig);
			//cv::setWindowProperty("Frame_", CV_WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
			cv::resize(imgOrig, imgOrig1, cv::Size(1920, 1200),0,0,cv::INTER_LINEAR);
			cv::imshow("Frame_1", imgOrig1);
			//cv::resizeWindow("Frame_",2448,2048);


			//cv::resize();
			
			cv::waitKey(30);

			char key = (char)cv::waitKey(1000);
			if (key == 'c')
			{

				std::string s = "D:\\Bhabhatron_gigE_calib\\Frame_" + std::to_string(i) + ".png";
				cv::imwrite(s, imgOrig);
				i++;
			}

			if (key == ESC_KEY)
			{
				cv::destroyAllWindows();
				return 0;
			}

		}

	}
	return 0;
}

