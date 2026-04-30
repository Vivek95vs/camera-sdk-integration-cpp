#include "stdafx.h"
#include "Grabber.h"

#define PIXEL_FORMAT BGR8


Grabber::Grabber()
{

	std::cout << "IN SecondaryFeedback_\t";
	startsystem();
	
}


Grabber::~Grabber()
{
}

void Grabber::startsystem()
{
	//readF = 0;

	//image = cvCreateImage(cv::Size(pBuffer->GetWidth(), pBuffer->GetHeight()), IPL_DEPTH_8U, 3);

	cv::FileStorage fsa("D:\\PMT_Radiotherapy\\Mlc\\camera_DB\\cam_mlc_props.xml", cv::FileStorage::READ);
	fsa["img_wd_req"] >> img_wd_req; //caqmera width requested
	fsa["img_ht_req"] >> img_ht_req;//camera height requested
	fsa.release();


	cv::FileStorage fs("D:\\PMT_Radiotherapy\\Mlc\\camera_DB\\camera_features.xml", cv::FileStorage::READ);
	fs["Gain1"] >> Gain1;
	fs["BlackLevel1"] >> BlackLevel1;
	fs["ExposureTime1"] >> ExposureTime1;
	fs["Brightness1"] >> Brightness1;
	fs["autoBrightnessMode1"] >> autoBright1;
	fs["flipX1"] >> flipX1;
	fs["flipY1"] >> flipY1;
	fs.release();

	cv::FileStorage flips("D:\\PMT_Radiotherapy\\Mlc\\camera_DB\\cam_mlc_props.xml", cv::FileStorage::READ); //,  OffY_roi,  Width_roi,  Height_roi;
																											 //flips["flipX"] >> flipX;
																											 //flips["flipY"] >> flipY;
	flips["exposure_factor"] >> exposure_factor;
	flips.release();



	std::cout << "\n Pheonix initialized successfully:" << "\n\n";

	std::cout << "Image Props" << " " << img_wd_req << " " << img_ht_req << " " << flipX1 << std::endl;
	bool exceptionThrown = false;
	try
	{
		pSystemtrig = Arena::OpenSystem();
		pSystemtrig->UpdateDevices(100);
		deviceInfos = pSystemtrig->GetDevices();

		if (deviceInfos.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return;
		}

		if (deviceInfos.size() > 0)
		{
			pDevice = pSystemtrig->CreateDevice(deviceInfos[0]);

			GenICam::gcstring acquisitionModeInitial1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");
			int64_t packet1 = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "GevSCPSPacketSize");
			GenICam::gcstring autogain1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "GainAuto");
			GenICam::gcstring exposureAutoInitial1 = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto");

			Arena::SetNodeValue<int64_t>(
				pDevice->GetNodeMap(),
				"GevSCPSPacketSize",
				9000);

			Arena::SetNodeValue<GenICam::gcstring>(
				pDevice->GetNodeMap(),
				"AcquisitionMode",
				"Continuous");


			reverseX1(flipX1);
			reverseY1(flipY1);

			Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width", img_wd_req);
			Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height", img_ht_req);

			Arena::SetNodeValue<GenICam::gcstring>(
				pDevice->GetNodeMap(),
				"GainAuto",
				"Off");

			Arena::SetNodeValue<GenICam::gcstring>(
				pDevice->GetNodeMap(),
				"ExposureAuto",
				"Off");

			//if (devicename1 == "CamA")
			//{
			success = setBlackLevel((double)BlackLevel1);
			//cout << "check for A lacklevel" << success << endl;
			success = setGain((double)Gain1);
			success = setExposureTime((double)ExposureTime1);
			//}

			Arena::SetNodeValue<GenICam::gcstring>(
				pDevice->GetTLStreamNodeMap(),
				"StreamBufferHandlingMode",
				"NewestOnly");

			if (GenApi::IsImplemented(pDevice->GetNodeMap()->GetNode("TCPEnable")))
			{

				Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "TCPEnable", true);
			}
			else
			{

				Arena::SetNodeValue<bool>(
					pDevice->GetTLStreamNodeMap(),
					"StreamPacketResendEnable",
					true);
			}


			pDevice->StartStream();

		}




		std::cout << "Camera--1 started Stream" << std::endl;

	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
		exceptionThrown = true;
		return;
	}
	catch (std::exception& ex)
	{
		std::cout << "\nStandard exception thrown: " << ex.what() << "\n";
		exceptionThrown = true;
		return;
	}
	catch (...)
	{
		std::cout << "\nUnexpected exception thrown\n";
		exceptionThrown = true;
		return;
	}

	if (exceptionThrown)
		return;

}

BOOL Grabber::reverseX1(BOOL revX1)
{
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "ReverseX", revX1);
	return 1;
}
BOOL Grabber::reverseY1(BOOL revY1)
{
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "ReverseY", revY1);
	return 1;

}

BOOL Grabber::setBlackLevel(double val)
{

	//cout << "Entering setBlackLevel" << " " << val << endl;
	BOOL status;
	double BlackLevelInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "BlackLevel");

	GenApi::CFloatPtr pBlackLevel = pDevice->GetNodeMap()->GetNode("BlackLevel");

	if (!pBlackLevel)
	{
		throw GenICam::GenericException("BlackLevel not found", __FILE__, __LINE__);
	}

	if (!GenApi::IsWritable(pBlackLevel))
	{
		throw GenICam::GenericException("Blacklevel node not writable", __FILE__, __LINE__);
	}

	double BlackLevelval = val; //(Min-0 to Max-12.5)
	pBlackLevel->SetValue(BlackLevelval);
	cv::setTrackbarPos("BlackLevel", "camera_Props", val);
	if (pBlackLevel)
	{
		status = true;
	}
	else
	{
		status = false;
	}
	//cout << "Finished setBlackLevel" << " " << status << endl;
	return status;
}

BOOL Grabber::setExposureTime(double ExposureTime)
{

	//cout << "Entering setExposureTime" << " " << ExposureTime << endl;
	BOOL status;
	double exposureTimeInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "ExposureTime");

	GenApi::CFloatPtr pExposureTime = pDevice->GetNodeMap()->GetNode("ExposureTime");

	if (!pExposureTime)
	{
		throw GenICam::GenericException("ExposureTime node not found", __FILE__, __LINE__);
	}

	if (!GenApi::IsWritable(pExposureTime))
	{
		throw GenICam::GenericException("ExposureTime node not writable", __FILE__, __LINE__);
	}

	double exposureTime = ExposureTime;

	pExposureTime->SetValue(exposureTime);
	cv::setTrackbarPos("ExposureTime", "camera_Props", exposureTime);
	if (pExposureTime)
	{
		status = true;
	}
	else
	{
		status = false;
	}
	//cout << "Finished setExposureTime" << " " << status << endl;
	return status;
}

BOOL Grabber::setGain(double G_val)
{
	//BOOL status = pAcq->SetFeatureValue("Gain", G_val);
	//cout << "Entering SetGain" << " " << G_val << endl;
	BOOL status;

	double gainInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "Gain");
	GenApi::CFloatPtr pgainraw = pDevice->GetNodeMap()->GetNode("Gain");

	if (!pgainraw)
	{
		throw GenICam::GenericException("Gain node not found", __FILE__, __LINE__);
	}

	if (!GenApi::IsWritable(pgainraw))
	{
		throw GenICam::GenericException("Gain node not writable", __FILE__, __LINE__);
	}

	double gainval = G_val; //(Min-0 to Max-48)
	pgainraw->SetValue(gainval);

	cv::setTrackbarPos("Gain", "camera_Props", G_val);
	if (pgainraw)
	{
		status = true;
	}
	else
	{
		status = false;
	}
	//cout << "Finished SetGain" << " " << status << endl;
	return status;
}

void Grabber::show_camera_Props()
{
	cv::namedWindow("camera_Props", cv::WINDOW_FREERATIO);
	cv::resizeWindow("camera_Props", 800, 300);
	cv::createTrackbar("Gain", "camera_Props", &Gain1, 48, onChange_camera, this); //   
	cv::createTrackbar("BlackLevel", "camera_Props", &BlackLevel1, 12, onChange_camera, this); //   
	cv::createTrackbar("ExposureTime", "camera_Props", &ExposureTime1, 28090, onChange_camera, this);
	cv::setTrackbarMin("ExposureTime", "camera_Props", 32);	

}

void Grabber::onChange_camera(int v, void *ptr)
{
	//resolving this
	Grabber *that = static_cast<Grabber*>(ptr);		// takes in void pointer and static_cast it to a Mlc_Cam type pointer
	that->realTrack_camera(v);
}

void Grabber::realTrack_camera(int v)
{

	int autoBright1 = 0;//  AutoBright Not Going to use...
	if (autoBright1 == 1) {
		// success= setautoBrightnessMode("Active");
		//UINT32 a = Brightness1;
		//success = setBrightness(a);
	}
	else {
		//	success = setautoBrightnessMode("Off");
		if (ExposureTime1 > 1)
		{
			success = setExposureTime((DOUBLE)ExposureTime1);
		}
	}

	if (Gain1 >= 1)
		success = setGain((double)Gain1);

	if (BlackLevel1 >1)
	{
		success = setBlackLevel((double)BlackLevel1);
	}

}

int Grabber::grabFrame(cv::Mat& frameA)
{
	//readF = 0;
	std::cout <<  "GrabFrame 1" << std::endl;

	//deviceInfos = pSystem->GetDevices();

	image1 = pDevice->GetImage(2000);

	int64_t width = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width");
	int64_t height = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");

	std::cout << "Image (w,h) = (" << width << "," << height << ") " << std::endl;

	pConvert1 = Arena::ImageFactory::Convert(image1, PIXEL_FORMAT);

	cv::Mat frame1 = cv::Mat((int)pConvert1->GetHeight(), (int)pConvert1->GetWidth(), CV_8UC3, (void *)pConvert1->GetData());

	frameA = frame1.clone();

	std::cout << "Frame1 size" << frame1.size() << std::endl;

	pDevice->RequeueBuffer(image1);
	std::cout << "GrabFrame1 completed" << std::endl;
	return 1;

	if (frameA.empty()) {
		std::cout << "Frame  empty" << std::endl;
		return 0;
	}
	else { return 1; }

}

int Grabber::isGigE_initialized()
{
	return 1;
}