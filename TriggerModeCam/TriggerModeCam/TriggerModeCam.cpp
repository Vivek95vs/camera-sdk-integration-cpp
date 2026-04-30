// TriggerModeCam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "GenTL\GenTL.h"
#include "Save\SaveApi.h"

#ifdef __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#include "GenICam.h"

#ifdef __linux__
#pragma GCC diagnostic pop
#endif

#include "Arena\ArenaApi.h"

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

int counter=0;
// Trigger: Introduction
//    This example introduces basic trigger configuration and use. In order to
//    configure trigger, enable trigger mode and set the source and selector. The
//    trigger must be armed before it is prepared to execute. Once the trigger is
//    armed, execute the trigger and retrieve an image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define TIMEOUT 2000

#define PIXEL_FORMAT BGR8



#define FILE_NAME "D:/Demo pic/image"
//
//// file type
#define FILE_TYPE ".png"


#define NUM_IMAGES 50

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates basic trigger configuration and use
// (1) sets trigger mode, source, and selector
// (2) starts stream
// (3) waits until trigger is armed
// (4) triggers image
// (5) gets image
// (6) requeues buffer
// (7) stops stream
void ConfigureTriggerAndAcquireImage(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring triggerLineSelectorInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "LineSelector");
	GenICam::gcstring triggerLineModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "LineMode");
	GenICam::gcstring triggerSelectorInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSelector");
	GenICam::gcstring triggerModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
	GenICam::gcstring triggerSourceInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource");
	GenICam::gcstring triggeractivationInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerActivation");
	double triggerdelayInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "TriggerDelay");

	// Set trigger selector
	//    Set the trigger selector to FrameStart. When triggered, the device will
	//    start acquiring a single frame. This can also be set to
	//    AcquisitionStart or FrameBurstStart.
	std::cout << TAB1 << "Set trigger selector to FrameStart\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"LineSelector","Line0");

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"LineMode","Input");

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"TriggerSelector","FrameStart");

	std::cout << TAB1 << "Enable trigger mode\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"TriggerMode","On");


	//std::cout << TAB1 << "Set trigger source to Software\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"TriggerSource","Line0");

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"TriggerActivation","RisingEdge");

	Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "TriggerDelay", 200000);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(),"AcquisitionMode","Continuous");

	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"GevSCPSPacketSize",
		9000);


	//// enable stream auto negotiate packet size
	Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(),"StreamAutoNegotiatePacketSize",true);

	//// enable stream packet resend
	Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(),"StreamPacketResendEnable",true);

	std::cout << TAB1 << "Start stream\n";
	int64_t packet = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "GevSCPSPacketSize");
	GenICam::gcstring LineSelector1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "LineSelector");
	GenICam::gcstring LineMode1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "LineMode");
	GenICam::gcstring TriggerSelector1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSelector");
	GenICam::gcstring TriggerMode1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
	GenICam::gcstring TriggerSource1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource");
	GenICam::gcstring TriggerActivation1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerActivation");
	GenICam::gcstring AcquisitionMode1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");
	std::cout << "Packet size" << packet << std::endl;

	std::cout << "LineSelector1" << " " << LineSelector1 << std::endl;
	std::cout << "LineMode1" << " " << LineMode1 << std::endl;
	std::cout << "TriggerSelector1" << " " << TriggerSelector1 << std::endl;
	std::cout << "TriggerMode1" << " " << TriggerMode1 << std::endl;
	std::cout << "TriggerSource1" << " " << TriggerSource1 << std::endl;
	std::cout << "TriggerActivation1" << " " << TriggerActivation1 << std::endl;
	std::cout << "AcquisitionMode1" << " " << AcquisitionMode1 << std::endl;

	//std::cout << TAB2 << "Wait until trigger is armed\n";

	

	//Arena::ExecuteNode(
	//	pDevice->GetNodeMap(),
	//	"TriggerSoftware");
	
	pDevice->StartStream();

	for (int j = 0;j <= 10;j++)
	{
		bool triggerArmed = false;
		do
		{
			triggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "LineStatus");
			std::cout << "triggerarmed" << " " << triggerArmed << std::endl;
		} while (triggerArmed == false);
		

		/*for (uint32_t i = 1; i <= NUM_IMAGES; i++)
		{*/
		// Trigger Armed
		//    Continually check until trigger is armed. Once the trigger is
		//    armed, it is ready to be executed.
		std::cout << TAB2 << "Wait until trigger is armed\n";
		//bool triggerArmed = false;

		//do
		//{
		//	triggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "TriggerArmed");
		//	std::cout << "triggerarmed" << " " << triggerArmed << std::endl;
		//} while (triggerArmed == false);
		std::cout << TAB2 << "Get image";

		Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);

		std::cout << " (" << pImage->GetWidth() << "x" << pImage->GetHeight() << ")\n";

		Arena::IImage* pConvert = Arena::ImageFactory::Convert(pImage, PIXEL_FORMAT);
		//// parameters required to save the image
		Save::ImageParams params(
			pConvert->GetWidth(),
			pConvert->GetHeight(),
			pConvert->GetBitsPerPixel());

		////std::cout << std::endl << " " << "Prepare image writer for image " << endl;

		//// naming each image in the order it were clicked
		std::string str = FILE_NAME + std::to_string(j) + FILE_TYPE;

		//// prepare image writer
		Save::ImageWriter writer(params, str.c_str());

		////// saves image
		writer << pConvert->GetData();

		//counter++;


		//std::cout << std::endl << " " << "Saved image " << endl;


		//Mat img = cv::Mat((int)pConvert->GetHeight(), (int)pConvert->GetWidth(), CV_8UC3, (void *)pConvert->GetData());

		std::cout << TAB2 << "Requeue buffer\n";

		pDevice->RequeueBuffer(pImage);
	}

		
		// Stop the stream
		std::cout << TAB1 << "Stop stream\n";
	//}

	//pDevice->StopStream();

	// return nodes to their initial values
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "LineSelector", triggerLineSelectorInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "LineMode", triggerLineModeInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource", triggerSourceInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode", triggerModeInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSelector", triggerSelectorInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerActivation", triggeractivationInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Trigger\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(1000);
		std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
		if (deviceInfos.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);
		GenICam::gcstring devicename = deviceInfos[0].ModelName();

		std::cout << "Camera Name" << " " << devicename << std::endl;

		// run example
		std::cout << "Commence example\n\n";
		for (int i = 0;i < 10;i++)
		{
			ConfigureTriggerAndAcquireImage(pDevice);
		}
		
		//pDevice->StopStream();
		//std::cout << "\nExample complete\n";

		//// clean up example
		//pSystem->DestroyDevice(pDevice);
		//Arena::CloseSystem(pSystem);
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