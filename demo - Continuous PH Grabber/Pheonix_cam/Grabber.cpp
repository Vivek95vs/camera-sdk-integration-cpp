#include "stdafx.h"
#include "Grabber.h"
#include<chrono>


#define PIXEL_FORMAT BGR8

// timeout for updating the device list
#define UPDATE_TIMEOUT 100

// timeout for grabbing an image
#define IMAGE_TIMEOUT 100


#define NUM_IMAGES 200

#define EXPOSURE_TIME 4000.0


#define TAB1 "  "
using namespace std;

Grabber::Grabber()
{
}


Grabber::~Grabber()
{
}


void Grabber::startsystem()
{


	std::cout << TAB1 << "Enumerate device\n";

	pSystem = Arena::OpenSystem();
	pSystem->UpdateDevices(100);
	std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();

	pDevice = pSystem->CreateDevice(deviceInfos[0]);

	GenICam::gcstring triggerModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
	GenICam::gcstring acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");


	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode", "Off");
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"AcquisitionMode",
		"Continuous");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetTLStreamNodeMap(),
		"StreamBufferHandlingMode",
		"NewestOnly");


	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"GevSCPSPacketSize",
		9000);


	//// enable stream auto negotiate packet size


	GenICam::gcstring autogain = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "GainAuto");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"GainAuto",
		"Continuous");


	GenICam::gcstring exposureAutoInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ExposureAuto",
		"Off");



	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetTLStreamNodeMap(),
		"StreamBufferHandlingMode",
		"NewestOnly");

	Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);

	//// enable stream packet resend
	Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

	//if (GenApi::IsImplemented(pDevice->GetNodeMap()->GetNode("TCPEnable")))
	//{

	//	std::cout << TAB1 << "Enable TCP stream\n";

	//	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "TCPEnable", true);
	//}
	//else
	//{

	//	std::cout << TAB1 << "Enable stream to auto negotiate packet size\n";

	//	Arena::SetNodeValue<bool>(
	//		pDevice->GetTLStreamNodeMap(),
	//		"StreamAutoNegotiatePacketSize",
	//		true);

	//	std::cout << TAB1 << "Enable stream packet resend\n";

	//	Arena::SetNodeValue<bool>(
	//		pDevice->GetTLStreamNodeMap(),
	//		"StreamPacketResendEnable",
	//		true);
	//}

	pDevice->StartStream();


}

void Grabber::GrabFrame()
{

	std::cout << TAB1 << "Getting " << NUM_IMAGES << " images\n";

	for (int i = 0; i < NUM_IMAGES; i++)
	{

		//pImage->GetWidth()=2000;
		pImage = pDevice->GetImage(IMAGE_TIMEOUT);


		//std::chrono::time_point<std::chrono::system_clock> start, end;
		//start = std::chrono::system_clock::now();
		int64_t width = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width");
		int64_t height = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");

		std::cout << TAB1 << "Image (w,h) = (" << width << "," << height << ") " << std::endl;

		uint64_t timestampNs = pImage->GetTimestampNs();


		pDevice->RequeueBuffer(pImage);

		std::cout << " and requeue\n";


	}

}

void Grabber::stopsystem()
{
	pDevice->StopStream();
	pSystem->DestroyDevice(pDevice);

	Arena::CloseSystem(pSystem);
}