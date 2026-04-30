#pragma once
#include "Arena\ArenaApi.h"
#include <conio.h>
#include<stdio.h>
#include "Save\SaveApi.h"

class Grabber
{
public:
	Grabber();
	~Grabber();

	void startsystem();
	void GrabFrame();
	void stopsystem();

	Arena::ISystem* pSystem;
	Arena::IDevice* pDevice;
	Arena::IImage* pImage;
	Arena::IImage* pConvert;

};

