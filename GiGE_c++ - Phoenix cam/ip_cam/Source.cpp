#include<iostream>
#include "mlc.h"
#include<time.h>
#include <windows.h>
#include<chrono>
/**
* @file Source1.cpp
* @author <b>PANACEA
* @date April 12 2017
* \mainpage MLC secondary feedback of leaf poitions
* MLC_CAM is used to......
* \section library_sec OpenCV Libraries used in this Project.
* \subsection OpenCV2 sstream deque
*/
#define CORSERVER_MAX_STRLEN		30
int main()
{
	clock_t start, end;
	double cpu_time;

	SEC_FB secfb;
	//cout << "sec_fb_Version " << secfb.sec_fb_version;
	secfb.display = true;//false;//
	//int rettt = MyGui.gigE.isGigE_initialized();
	char acqServerName[CORSERVER_MAX_STRLEN] = "";
	int rettt = secfb.gigEserverFound(acqServerName);



	//double t = MyGui.deviceTemp();

	if (rettt == 0)
	{
		secfb.gigE.DestroyObjects();
		return 0;
	}

	return 0;

}