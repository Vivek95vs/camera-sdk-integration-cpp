#pragma once
#include <Windows.h>
#include <iostream>
#include "Basler_acer.h"

struct MyContext
{
	// Define some application specific context information here
};

class Grabber
{
public:
	Grabber();
	~Grabber();

	void call();
	void ProcessImage(unsigned char * pImage, int imageSizeX, int imageSizeY);

	
};


