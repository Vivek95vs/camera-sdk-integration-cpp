// Basler_acer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Basler_acer.h"


typedef CBaslerGigECamera Camera_t;
static const uint32_t c_maxCamerasToUse = 2;
static const uint32_t c_countOfImagesToGrab = 10;


int main()
{

	// The exit code of the sample application.
	int exitCode = 0;

	Grabber g;

	g.call();

	// Before using any pylon methods, the pylon runtime must be initialized.
	PylonInitialize();

	try
	{
		// Get the transport layer factory.
		CTlFactory& tlFactory = CTlFactory::GetInstance();

		// Get all attached devices and exit application if no device is found.
		DeviceInfoList_t devices;
		if (tlFactory.EnumerateDevices(devices) == 0)
		{
			throw RUNTIME_EXCEPTION("No camera present.");
		}

		// Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.
		CInstantCameraArray cameras(min(devices.size(), c_maxCamerasToUse));

		cout << "Camera device Number" << cameras.GetSize();

		// Create and attach all Pylon Devices.
		for (size_t i = 0; i < cameras.GetSize(); ++i)
		{
			cameras[i].Attach(tlFactory.CreateDevice(devices[i]));

			// Print the model name of the camera.
			cout << "Using device " << cameras[i].GetDeviceInfo().GetModelName<< endl;

		}


		// Starts grabbing for all cameras starting with index 0. The grabbing
		// is started for one camera after the other. That's why the images of all
		// cameras are not taken at the same time.
		// However, a hardware trigger setup can be used to cause all cameras to grab images synchronously.
		// According to their default configuration, the cameras are
		// set up for free-running continuous acquisition.
		cameras.StartGrabbing();

		// This smart pointer will receive the grab result data.
		CGrabResultPtr ptrGrabResult;

		// Grab c_countOfImagesToGrab from the cameras.
		for (uint32_t i = 0; i < c_countOfImagesToGrab && cameras.IsGrabbing(); ++i)
		{
			cameras.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{
				// When the cameras in the array are created the camera context value
				// is set to the index of the camera in the array.
				// The camera context is a user settable value.
				// This value is attached to each grab result and can be used
				// to determine the camera that produced the grab result.
				intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();

#ifdef PYLON_WIN_BUILD
				// Show the image acquired by each camera in the window related to each camera.
				Pylon::DisplayImage(cameraContextValue, ptrGrabResult);
#endif

				// Print the index and the model name of the camera.
				cout << "Camera " << cameraContextValue << ": " << cameras[cameraContextValue].GetDeviceInfo().GetModelName() << endl;

				// Now, the image data can be processed.
				cout << "GrabSucceeded: " << ptrGrabResult->GrabSucceeded() << endl;
				cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
				cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
				const uint8_t* pImageBuffer = (uint8_t*)ptrGrabResult->GetBuffer();
				cout << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0] << endl << endl;
			}
			else
			{
				cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
			}
		}
	}
	catch (const GenericException& e)
	{
		// Error handling
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
		exitCode = 1;
	}

	// Comment the following two lines to disable waiting on exit.
	cerr << endl << "Press enter to exit." << endl;
	while (cin.get() != '\n');

	// Releases all pylon resources.
	PylonTerminate();

	return exitCode;




}

