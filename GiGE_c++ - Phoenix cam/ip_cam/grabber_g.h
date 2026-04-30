#pragma once
//#include "allVars.h"
#include <iostream>

//#include "SapClassBasic.h"
#include "Arena\ArenaApi.h"
#include "Save\SaveApi.h"
#include "GenICam.h"

static bool readF;
class Grabber
{
	int enumCount; int enumIndex, enumValue; char enumStr[64];
public:


	bool isGigE_init;
	int isGigE_initialized();

	BOOL success/*, status*/;

	Grabber();
	Grabber(char server_name[64] /*= "Nano-C2050_1"*/, char config_file[64] = NULL);

	~Grabber();
	void DestroyObjects();

	//IplImage* image;

	Arena::ISystem* pSystem;
	Arena::IDevice* pDevice;
	Arena::IImage* image;
	Arena::IImage* pConvert;
	std::vector<Arena::DeviceInfo> deviceInfos;


	void grabFrame();
	void grabFrame_trigger();

	void startsystem();
	void Closesystem();
};