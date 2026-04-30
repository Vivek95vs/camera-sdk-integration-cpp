#pragma once
#include "Arena\ArenaApi.h"
#include <conio.h>
#include<stdio.h>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"
#include <opencv2/opencv.hpp>
#include "Save\SaveApi.h"

class Grabber
{
public:
	Grabber();
	~Grabber();
	int Brightnessval, Gainval, BlackLevelval1, ExposureTimeval;			//!< camera properties
	int OffsetX, OffsetY, Width, Height,width,height;
	void show_camera_Props();
	static void onChange_camera(int v, void *ptr);
	void realTrack_camera(int v);
	void EnumerateDeviceAndAcquireImage();

	Arena::ISystem* pSystem;
	Arena::IDevice* pDevice;
	Arena::IImage* pImage;
	Arena::IImage* pConvert;

	BOOL success;
	BOOL setGain(double G_val);
	BOOL setBlackLevel(double val);
	BOOL setExposureTime(double ExposureTime);
};

