// camera_write.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "camselect.h"


int main()
{
	camselect cam;

	cam.connectdatabase();

	//Write Data To SQL
	//cam.insertcamdetails();
	//cam.BankB_mm_per_pixels();
	//cam.lambdacreate();
	//cam.cornerpoints();
	//cam.rect();
	//cam.Bank_min_max();
	//cam.Bank_pixels();
	//cam.cam_mlc_props();
	//cam.cam_features();
	//cam.BankA_mm_per_pixels();
	//cam.BankA_points();
	//cam.BankB_points();
	//cam.distance_moved_mm();
	//cam.image_props_Trackbars();

	//Fetch DATA
	//cam.fetchpixels();
	//cam.fetchcamera();
	//cam.camserialnumberwrite();
	//cam.distance_moved_mm();

	//cam.bankpoints();
	cam.bankpointsvec();
	getchar();

	return 0;
}

