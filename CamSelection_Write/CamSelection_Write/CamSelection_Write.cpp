// sqlfetch_VS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "camselect.h"


int main()
{

	camselect cam;

	cam.connectdatabase();
	//cam.insertcamdetails();
	//cam.BankB_mm_per_pixels();
	//cam.lambdacreate();
	//cam.cornerpoints();
	//cam.rect();
	//cam.Bank_min_max();
	//cam.Bank_pixels();
	//cam.cam_mlc_props();
	//cam.cam_features();
	cam.BankA_mm_per_pixels();

	getchar();

	return 0;
}

