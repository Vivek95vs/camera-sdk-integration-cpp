

#include "stdafx.h"
#include <iostream>
#include "GenICam.h"
#include "Arena\ArenaApi.h"
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"
#include <opencv2/opencv.hpp>

#define TAB1 "  "
#define UPDATE_TIMEOUT 100
#define IMAGE_TIMEOUT 2000

using namespace cv;
using namespace std;
int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_SimpleAcquisition\n";

	try
	{
		// run example
		std::cout << "Commence example\n\n";
		// Enumerate device
		// Starting Arena just requires opening the system. From there, update and
		// grab the device list, and create the device. Notice that failing to
		// update the device list will return an empty list, even if devices are
		// connected.
		std::cout << TAB1 << "Enumerate device\n";

		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(100);
		std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
		std::cout << "DEViceinfo        " << deviceInfos[0].ModelName()<<std::endl;
		std::cout << "SerialNumber      " << deviceInfos[0].SerialNumber()<<std::endl;
		std::cout << "IpAddress    " << deviceInfos[0].IpAddress() << endl;


		if (deviceInfos.size() > 0)
		{
			Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

			// enable stream auto negotiate packet size
			//Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);

			//// enable stream packet resend
			//Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

			// Acquire image
			// Once a device is created, it is only a single call to acquire an
			// image. The timeout must be larger than the exposure time.
			GenApi::INodeMap *pNodeMap = pDevice->GetNodeMap();
			//GenICam::gcstring windowName = Arena::GetNodeValue<GenICam::gcstring>(pNodeMap, "DeviceModelName") + " (" + Arena::GetNodeValue(pNodeMap, "DeviceSerialNumber") + ")";

			Arena::IImage *pImage;
			cv::Mat img;

			int framesPerSecond = (int)Arena::GetNodeValue<double>(pNodeMap, "AcquisitionFrameRate");
			int numberOfSeconds = 10;

			GenApi::CFloatPtr pExposureTimeNode = pNodeMap->GetNode("ExposureTime");
			const int64_t getImageTimeout_ms = static_cast<int64_t>(pExposureTimeNode->GetMax() / 1000 * 2);

			std::cout << "Starting stream." << std::endl;
			pDevice->StartStream();

			for (int i = 0; i < framesPerSecond * numberOfSeconds; i++)
			{
				pImage = pDevice->GetImage(getImageTimeout_ms);
				img = cv::Mat((int)pImage->GetHeight(), (int)pImage->GetWidth(), CV_8UC1, (void *)pImage->GetData());
				cv::imshow("grabber", img);
				cv::waitKey(2);
				pDevice->RequeueBuffer(pImage);
			}
			//cv::destroyAllWindows();

		
			// Stop stream
			std::cout << "Stopping stream." << std::endl;
			pDevice->StopStream();

		}

		Arena::CloseSystem(pSystem);
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
    return 0;
}

