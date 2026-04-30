#include "stdafx.h"
#include "Grabber.h"
#include<chrono>

// Simple Acquisition
//    This examples demonstrates the most basic code path of acquiring an image
//    using Arena SDK. This includes device enumeration, image acquisition, and
//    clean up.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

#define PIXEL_FORMAT BGR8

// timeout for updating the device list
#define UPDATE_TIMEOUT 100

// timeout for grabbing an image
#define IMAGE_TIMEOUT 100

#define FILE_NAME "D:/Demo pic/image"
//
//// file type
#define FILE_TYPE ".png"

#define NUM_IMAGES 2000

#define EXPOSURE_TIME 4000.0

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates simplest route to acquiring an image
// (1) enumerates device
// (2) acquires image
// (3) cleans up

#define TAB1 "  "
using namespace std;
using namespace cv;

Grabber::Grabber()
{
}


Grabber::~Grabber()
{
}

void Grabber::show_camera_Props()
{
	cv::namedWindow("camera_Props", cv::WINDOW_AUTOSIZE);
	//cv::setWindowProperty("camera_Props", cv::WND_PROP_ASPECT_RATIO,cv::WINDOW_AUTOSIZE);
	cv::resizeWindow("camera_Props", 500, 150);
	//cv::createTrackbar("setautoBrightnessMode", "camera_Props", &autoBright, 1, onChange_camera, this); //saturation minimum
	cv::createTrackbar("Gain", "camera_Props", &Gainval, 48, onChange_camera, this); //  value minimum
																				 //cv::createTrackbar("Brightness", "camera_Props", &Brightness, 255, onChange_camera, this); //saturation minimum
	cv::createTrackbar("BlackLevel", "camera_Props", &BlackLevelval1, 12, onChange_camera, this); //  value minimum
	//cv::createTrackbar("ExposureTime", "camera_Props", &ExposureTimeval, 7781, onChange_camera, this); //brightness
	//cv::setTrackbarMin("ExposureTime", "camera_Props", 34);
																								   //cv::createTrackbar("Restore values", "camera_Props", &restore_track, 1, onChange_camera, this);//to restore default values of above paras
}

void Grabber::onChange_camera(int v, void *ptr)
{
	//resolving this
	Grabber *that = static_cast<Grabber*>(ptr);		// takes in void pointer and static_cast it to a Mlc_Cam type pointer
	that->realTrack_camera(v);
}
BOOL Grabber::setBlackLevel(double val)
{

	cout << "Entering setBlackLevel" << " " << val << endl;
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
	cout << "Finished setBlackLevel" << " " << status << endl;
	return status;
}

BOOL Grabber::setExposureTime(double ExposureTime)
{

	cout << "Entering setExposureTime" << " " << ExposureTime << endl;
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

	double exposureTime = 7781;

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
	cout << "Finished setExposureTime" << " " << status << endl;
	return status;
}

BOOL Grabber::setGain(double G_val)
{
	//BOOL status = pAcq->SetFeatureValue("Gain", G_val);
	cout << "Entering SetGain" << " " << G_val << endl;
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
	cout << "Finished SetGain" << " " << status << endl;
	return status;
}

void Grabber::realTrack_camera(int v)												//
{
	BOOL sta;

	sta = setGain((double)Gainval);
	sta = setBlackLevel((double)BlackLevelval1);
	//sta = setExposureTime((double)ExposureTimeval);

	cv::FileStorage fs("D:\\PMT_Radiotherapy\\Mlc\\Pheonix cam\\camera_features.xml", cv::FileStorage::WRITE);		// writes trackbar values to xml for retrieving current track bar values in next run												// write 
	fs << "Gain" << Gainval;
	//fs << "iLowH2" << iLowH2;
	//fs << "iHighH2" << iHighH2;
	fs << "Brightness" << Brightnessval;
	fs << "BlackLevel" << BlackLevelval1;
	fs << "ExposureTime" << ExposureTimeval;
	fs.release();
	cv::waitKey(10);

	//showTrackbars();
}

void Grabber::EnumerateDeviceAndAcquireImage()
{

	cv::FileStorage feats("D:\\PMT_Radiotherapy\\Mlc\\Pheonix cam\\camera_features.xml", cv::FileStorage::READ);
	//feats["autoBrightnessMode"] >> autoBright;
	feats["Gain"] >> Gainval;
	//feats["Brightness"] >> Brightness;
	feats["BlackLevel"] >> BlackLevelval1;
	feats["ExposureTime"] >> ExposureTimeval;
	feats["OffsetX"] >> OffsetX;
	feats["OffsetY"] >> OffsetY;
	feats["Width"] >> Width;
	feats["Height"] >> Height;
	//feats["restore_track"] >> restore_track;
	feats.release();

	cv::FileStorage feat("D:\\PMT_Radiotherapy\\Mlc\\camera_DB\\Bh_cam_mlc_props.xml", cv::FileStorage::READ);
	//feats["autoBrightnessMode"] >> autoBright;
	feat["img_wd_req"] >> width;
	//feats["Brightness"] >> Brightness;
	feat["img_ht_req"] >> height;
	//feats["restore_track"] >> restore_track;
	feat.release();

	std::cout << "Acquire device value" << " " << Gainval << " " << BlackLevelval1 << " " << ExposureTimeval << " " << endl;
	std::cout << "Cam Properties" << " " << OffsetX << " " << OffsetY << " " << Width << " " << Height <<endl;
	//cv::setTrackbarPos("Gain", "camera_Props", Gainval);
	//cv::setTrackbarPos("ExposureTime", "camera_Props", ExposureTimeval);
	//cv::setTrackbarPos("BlackLevel", "camera_Props", BlackLevelval1);

	std::cout << TAB1 << "Enumerate device\n";

	pSystem = Arena::OpenSystem();
	pSystem->UpdateDevices(100);
	std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();

	pDevice = pSystem->CreateDevice(deviceInfos[0]);

	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "UserDefinedName", "CamA");
	//std::string devicename = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "UserDefinedName").c_str();
	//deviceInfos[0].UserDefinedName(). = "CamA";
	//GenICam::gcstring ipStr = deviceInfos[0].UserDefinedName();

	//cout << "name of device" << " " << ipStr << endl;

	GenICam::gcstring acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"AcquisitionMode",
		"Continuous");

	GenICam::gcstring autogain = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "GainAuto");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"GainAuto",
		"Off");

	Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width", width);
	Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height", height);


	GenICam::gcstring exposureAutoInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ExposureAuto",
		"Off");

	cout << "value" << " " << BlackLevelval1 << " " << Gainval << " " << ExposureTimeval << endl;
	success = setBlackLevel((double)BlackLevelval1);
	cout << "check for b lacklevel" << success << endl;
	success = setGain((double)Gainval);
	//success = setExposureTime((double)ExposureTimeval);

	//Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "OffsetX", 250);
	//Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "OffsetY", 150);
	//Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width", 200);
	//Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height", 200);


	//int64_t width = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width");
	//int64_t height = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");
	//double  Offsetx1 = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "OffsetX");
	//double Offsety1 = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "OffsetY");

	//std::cout << TAB1 << "Image (w,h) = (" << width << "," << height << ") " << Offsetx1<<" "<< Offsety1<<" "<< std::endl;



	//Arena::FeatureStream featureStream(pDevice->GetNodeMap());
	//featureStream.Read("D:\\PMT_Radiotherapy\\Mlc\\camera_DB\\features.txt");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetTLStreamNodeMap(),
		"StreamBufferHandlingMode",
		"NewestOnly");

	if (GenApi::IsImplemented(pDevice->GetNodeMap()->GetNode("TCPEnable")))
	{

		std::cout << TAB1 << "Enable TCP stream\n";

		Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "TCPEnable", true);
	}
	else
	{

		std::cout << TAB1 << "Enable stream to auto negotiate packet size\n";

		Arena::SetNodeValue<bool>(
			pDevice->GetTLStreamNodeMap(),
			"StreamAutoNegotiatePacketSize",
			true);

		std::cout << TAB1 << "Enable stream packet resend\n";

		Arena::SetNodeValue<bool>(
			pDevice->GetTLStreamNodeMap(),
			"StreamPacketResendEnable",
			true);
	}

	//std::chrono::time_point<std::chrono::system_clock> start, end;
	//start = std::chrono::system_clock::now();
	pDevice->StartStream();
	//end = std::chrono::system_clock::now();

	//std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "Start Stream: " << elapsed_seconds.count() << "s\n";

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

		pConvert = Arena::ImageFactory::Convert(pImage, PIXEL_FORMAT);
		// parameters required to save the image
		Save::ImageParams params(
			pImage->GetWidth(),
			pImage->GetHeight(),
			pImage->GetBitsPerPixel());

		//std::cout << std::endl << " " << "Prepare image writer for image " << endl;

		// naming each image in the order it were clicked
		std::string str = FILE_NAME + std::to_string(i) + FILE_TYPE;

		// prepare image writer
		Save::ImageWriter writer(params, str.c_str());

		//// saves image
		writer << pImage->GetData();


		//success = setGain((double)Gainval);
		//success = setBlackLevel((double)BlackLevelval1);
		//success = setExposureTime((double)ExposureTimeval);

		//std::cout << std::endl << " " << "Saved image " << endl;


		Mat img = cv::Mat((int)pConvert->GetHeight(), (int)pConvert->GetWidth(), CV_8UC3, (void *)pConvert->GetData());
		//end = std::chrono::system_clock::now();

		//std::chrono::duration<double> elapsed_seconds = end - start;
		//std::cout << "Mat Fun: " << elapsed_seconds.count() << "s\n";
		//double angle = 0;
		//Mat dst;      //Mat object for output image file
		//Point2f pt(img.cols / 2., img.rows / 2.);          //point from where to rotate    
		//Mat r = getRotationMatrix2D(pt, -180, 1.0);      //Mat object for storing after rotation
		//warpAffine(img, img, r, Size(img.cols, img.rows));
		cv::namedWindow("strips", cv::WINDOW_NORMAL);
		cv::setWindowProperty("strips", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		//cv::imwrite("output.png", img);
		cv::imshow("strips", img);
		cv::waitKey(10);

		//cv::destroyAllWindows;


		//std::cout << TAB1 << "Clean up Arena\n" << img.size();
		//Arena::ImageFactory::Destroy(pConvert);
		pDevice->RequeueBuffer(pImage);

		std::cout << " and requeue\n";


	}

	pDevice->StopStream();

	//std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	//std::cout << "finished computation at " << std::ctime(&end_time)

	//Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "Gain", gainInitial);
	//Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "BlackLevel", BlackLevelInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", acquisitionModeInitial);
	//Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", acquisitionModeInitial);

	//}
	pSystem->DestroyDevice(pDevice);

	Arena::CloseSystem(pSystem);
}
