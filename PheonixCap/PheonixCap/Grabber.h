#pragma once
#include <iostream>
#include <direct.h>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include "Arena\ArenaApi.h"
#include "Save\SaveApi.h"
#include <string>


class Grabber
{
public:
	Grabber();
	~Grabber();

	void startsystem();
	int img_wd_req;							//!<  colomns requested to camera
	int img_ht_req;
	int img_wd=1000;
	int img_ht=1000;
	int Gain1, BlackLevel1, ExposureTime1, Brightness1, autoBright1, flipX1, flipY1, exposure_factor;

	Arena::ISystem* pSystemtrig;
	std::vector<Arena::DeviceInfo> deviceInfos;
	Arena::IDevice* pDevice;
	Arena::IImage* image1;
	Arena::IImage* pConvert1;
	int grabFrame(cv::Mat& frameA);
	BOOL reverseX1(BOOL revX1);

	BOOL reverseY1(BOOL revY1);
	BOOL success;
	BOOL setGain(double G_val);
	BOOL setBlackLevel(double val);
	BOOL setExposureTime(double ExposureTime);
	void show_camera_Props();
	static void onChange_camera(int v, void *ptr);
	void realTrack_camera(int v);
	int isGigE_initialized();


};

