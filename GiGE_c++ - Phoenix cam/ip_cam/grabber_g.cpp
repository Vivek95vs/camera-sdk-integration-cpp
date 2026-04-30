#include "grabber_g.h"

#define TAB1 "  "

#define PIXEL_FORMAT BGR8

// timeout for updating the device list
#define UPDATE_TIMEOUT 100

// timeout for grabbing an image
#define IMAGE_TIMEOUT 2000

#define FILE_NAME "D:/Demo pic/image"
//
//// file type
#define FILE_TYPE ".png"




void Grabber::startsystem()
{
	readF = 0;
	std::cout << TAB1 << "Enumerate device\n";
	pSystem= Arena::OpenSystem();
	pSystem->UpdateDevices(100);
	deviceInfos = pSystem->GetDevices();
	std::cout << "total device connected" << deviceInfos.size();
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
	Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);

	//// enable stream packet resend
	Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);
	pDevice->StartStream();

	std::cout << TAB1 << "Acquire image\n";
}

void Grabber::Closesystem()
{
	pDevice->StopStream();

}


//============================================================================

Grabber::Grabber() {	}

Grabber::Grabber(char server_name[64], char config_file[64])
{





}

Grabber::~Grabber()
{}
void Grabber::DestroyObjects() {


}


//__________________________________________________________________________________
int Grabber::isGigE_initialized()
{
	return isGigE_init;
}
void Grabber::grabFrame_trigger()
{

	readF = 0;


	std::cout << "  " << "Get image ";
	//	Arena::IImage* image1 = pDevice1->GetImage(2000);
	image = pDevice->GetImage(IMAGE_TIMEOUT);

	pConvert = Arena::ImageFactory::Convert(image, PIXEL_FORMAT);

	//uint64_t timestampNs = image->GetTimestampNs();

	Save::ImageParams params(
		pConvert->GetWidth(),
		pConvert->GetHeight(),
		pConvert->GetBitsPerPixel());

	////std::cout << std::endl << " " << "Prepare image writer for image " << endl;

	//// naming each image in the order it were clicked
	std::string str = FILE_NAME + std::to_string(10) + FILE_TYPE;

	//// prepare image writer
	Save::ImageWriter writer(params, str.c_str());

	////// saves image
	writer << pConvert->GetData();

	std::cout << std::endl << " " << "Saved image " << std::endl;



	std::cout << TAB1 << "Clean up Arena\n";
	//	Arena::ImageFactory::Destroy(pConvert);
	pDevice->RequeueBuffer(image);

	std::cout << " and requeue\n";

	//_____________________________________________________readF = 0-----v______frame acquired__do processing with it

}
//_____________________________________________________________________________________________________________________________________

void Grabber::grabFrame()
{

	readF = 0;


	std::cout << "  " << "Get image ";
	//	Arena::IImage* image1 = pDevice1->GetImage(2000);
	image = pDevice->GetImage(2000);

	pConvert = Arena::ImageFactory::Convert(image, PIXEL_FORMAT);

	//uint64_t timestampNs = image->GetTimestampNs();
	Save::ImageParams params(
		pConvert->GetWidth(),
		pConvert->GetHeight(),
		pConvert->GetBitsPerPixel());

	////std::cout << std::endl << " " << "Prepare image writer for image " << endl;

	//// naming each image in the order it were clicked
	std::string str = FILE_NAME + std::to_string(10) + FILE_TYPE;

	//// prepare image writer
	Save::ImageWriter writer(params, str.c_str());

	////// saves image
	writer << pConvert->GetData();


	std::cout << std::endl << " " << "Saved image " << std::endl;



	std::cout << TAB1 << "Clean up Arena\n";
	//	Arena::ImageFactory::Destroy(pConvert);
	pDevice->RequeueBuffer(image);

	std::cout << " and requeue\n";

	//_____________________________________________________readF = 0-----v______frame acquired__do processing with it

}
