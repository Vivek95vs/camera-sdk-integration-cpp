#include "stdafx.h"
#include "Arena\ArenaApi.h"
#include "Save\SaveApi.h"
#include <conio.h>
#include<stdio.h>
#include<chrono>
#include <time.h>
//#include "opencv2/core.hpp"
//#include <opencv2/core/utility.hpp>
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/cudaimgproc.hpp"
//#include <opencv2/opencv.hpp>
//#include "Save\SaveApi.h"
//#include "GenICam.h"


#define TAB1 "  "
using namespace std;
//using namespace cv;


#define PIXEL_FORMAT BGR8

// timeout for updating the device list
#define UPDATE_TIMEOUT 100

// timeout for grabbing an image
#define IMAGE_TIMEOUT 2000

#define NUM_IMAGES 2000

#define FILE_NAME "D:/Demo pic/image"
//
//// file type
#define FILE_TYPE ".png"

void EnumerateDeviceAndAcquireImage()
{
	std::cout << TAB1 << "Enumerate device\n";
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	Arena::ISystem* pSystem = Arena::OpenSystem();
	pSystem->UpdateDevices(100);
	std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();

	cout << "total device connected" << deviceInfos.size();

	if (deviceInfos.size() > 0)
	{
		Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

		GenICam::gcstring triggerModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
		GenICam::gcstring acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");
	

		Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode", "Off");

		Arena::SetNodeValue<GenICam::gcstring>(
			pDevice->GetNodeMap(),
			"AcquisitionMode",
			"Continuous");

		//Arena::SetNodeValue<GenICam::gcstring>(
		//	pDevice->GetTLStreamNodeMap(),
		//	"StreamBufferHandlingMode",
		//	"NewestOnly");

		//Arena::SetNodeValue<int64_t>(
		//	pDevice->GetNodeMap(),
		//	"GevSCPSPacketSize",
		//	9000);

		string serverSerialNumberA;
		serverSerialNumberA = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceSerialNumber").c_str();

		Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceUserID", "Device1");
		GenICam::gcstring name = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceUserID");
		std::cout << "name" << name<<" "<< serverSerialNumberA;


		//// enable stream auto negotiate packet size
		Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);

		//// enable stream packet resend
		Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

		pDevice->StartStream();


		std::cout << TAB1 << "Getting " << NUM_IMAGES << " images\n";

			Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);

			int64_t width = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width");
			int64_t height = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");

			std::cout << TAB1 << "Image (w,h) = (" << width << "," << height << ") " << std::endl;

			uint64_t timestampNs = pImage->GetTimestampNs();

			Arena::IImage* pConvert = Arena::ImageFactory::Convert(pImage, PIXEL_FORMAT);
			// parameters required to save the image
			Save::ImageParams params(pConvert->GetWidth(),pConvert->GetHeight(),pConvert->GetBitsPerPixel());

			std::string str = FILE_NAME + std::to_string(2) + FILE_TYPE;

			//// prepare image writer
			Save::ImageWriter writer(params, str.c_str());

			//// saves image
			writer << pConvert->GetData();
			pDevice->RequeueBuffer(pImage);


			std::cout << " and requeue\n";

		//pDevice->StopStream();
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout<< "elapsed time: " << elapsed_seconds.count() << "s\n";
		//pSystem->DestroyDevice(pDevice);
	}
	//Arena::CloseSystem(pSystem);
}


int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_SimpleAcquisition\n";

	try
	{
		// run example
		std::cout << "Commence example\n\n";
		EnumerateDeviceAndAcquireImage();
		std::cout << "\nExample complete\n";
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
		exceptionThrown = true;
	}
	catch (std::exception& ex)
	{
		std::cout << "Standard exception thrown: " << ex.what() << "\n";
		exceptionThrown = true;
	}
	catch (...)
	{
		std::cout << "Unexpected exception thrown\n";
		exceptionThrown = true;
	}

	std::cout << "Press enter to complete\n";
	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}