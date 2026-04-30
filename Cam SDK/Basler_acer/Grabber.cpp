#include "stdafx.h"
#include "Grabber.h"
#include "Basler_acer.h"

typedef CBaslerGigECamera Camera_t;

using namespace Basler_GigECameraParams;


Grabber::Grabber()
{
}


Grabber::~Grabber()
{
}

void Grabber::ProcessImage(unsigned char* pImage, int imageSizeX, int imageSizeY)
{
	// Do something with the image data
}
void Grabber::call() {

	PylonAutoInitTerm autoInitTerm;

	try
	{
		// Enumerate GigE cameras
		CTlFactory& TlFactory = CTlFactory::GetInstance();
		ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());
		DeviceInfoList_t devices;
		if (0 == pTl->EnumerateDevices(devices)) {
			cerr << "No camera present!" << endl;
			return;
		}

		// Create a camera object
		Camera_t camera(pTl->CreateDevice(devices[2]));

		// Open the camera object
		camera.Open();

		// Parameterize the camera

		// Mono8 pixel format
		camera.PixelFormat.SetValue(Basler_GigECameraParams::PixelFormat_Mono8);

		//camera.PixelFormat.SetValue

		// Maximized AOI
		camera.OffsetX.SetValue(0);
		camera.OffsetY.SetValue(0);
		camera.Width.SetValue(camera.Width.GetMax());
		camera.Height.SetValue(camera.Height.GetMax());

		// Continuous mode, no external trigger used
		camera.TriggerSelector.SetValue(Basler_GigECameraParams::TriggerSelector_AcquisitionStart);
		camera.TriggerMode.SetValue(Basler_GigECameraParams::TriggerMode_On);
		camera.TriggerSource.SetValue(Basler_GigECameraParams::TriggerSource_Line1);
		camera.AcquisitionMode.SetValue(Basler_GigECameraParams::AcquisitionMode_Continuous);


		// Configure exposure time and mode
		camera.ExposureMode.SetValue(Basler_GigECameraParams::ExposureMode_Timed);
		camera.ExposureTimeRaw.SetValue(100);

		// check whether stream grabbers are avalaible
		if (camera.GetNumStreamGrabberChannels() == 0) {
			cerr << "Camera doesn't support stream grabbers." << endl;
		}
		else {
			// Get and open a stream grabber
			IStreamGrabber* pGrabber = camera.GetStreamGrabber(0);
			CBaslerGigECamera::StreamGrabber_t StreamGrabber(camera.GetStreamGrabber(0));
			StreamGrabber.Open();

			// Parameterize the stream grabber
			const int bufferSize = (int)camera.PayloadSize();
			const int numBuffers = 10;
			StreamGrabber.MaxBufferSize = bufferSize;
			StreamGrabber.MaxNumBuffer = numBuffers;
			StreamGrabber.PrepareGrab();

			// Allocate and register image buffers, put them into the
			// grabber's input queue
			unsigned char* ppBuffers[numBuffers];
			MyContext context[numBuffers];
			StreamBufferHandle handles[numBuffers];
			for (int i = 0; i < numBuffers; ++i)
			{
				ppBuffers[i] = new unsigned char[bufferSize];
				handles[i] = StreamGrabber.RegisterBuffer(ppBuffers[i], bufferSize);
				StreamGrabber.QueueBuffer(handles[i], &context[i]);
			}

			// Start image acquisition
			camera.AcquisitionStart.Execute();

			// Grab and process 100 images
			const int numGrabs = 100;
			GrabResult Result;
			for (int i = 0; i < numGrabs; ++i) {
				// Wait for the grabbed image with a timeout of 3 seconds
				if (StreamGrabber.GetWaitObject().Wait(3000)) {
					// Get an item from the grabber's output queue
					if (!StreamGrabber.RetrieveResult(Result)) {
						cerr << "Failed to retrieve an item from the output queue" << endl;
						break;
					}
					if (Result.Succeeded()) {
						// Grabbing was successful. Process the image.
						ProcessImage((unsigned char*)Result.Buffer(), Result.GetSizeX(), Result.GetSizeY());
					}
					else {
						cerr << "Grab failed: " << Result.GetErrorDescription() << endl;
						break;
					}
					// Requeue the buffer
					if (i + numBuffers < numGrabs)
						StreamGrabber.QueueBuffer(Result.Handle(), Result.Context());
				}
				else {
					cerr << "timeout occurred when waiting for a grabbed image" << endl;
					break;
				}
			}

			// Finished. Stop grabbing and do clean-up

			// The camera is in continuous mode, stop image acquisition
			camera.AcquisitionStop.Execute();

			// Flush the input queue, grabbing may have failed
			StreamGrabber.CancelGrab();

			// Consume all items from the output queue
			while (StreamGrabber.GetWaitObject().Wait(0)) {
				StreamGrabber.RetrieveResult(Result);
				if (Result.Status() == Canceled)
					cout << "Got canceled buffer" << endl;
			}

			// Deregister and free buffers
			for (int i = 0; i < numBuffers; ++i) {
				StreamGrabber.DeregisterBuffer(handles[i]);
				delete[] ppBuffers[i];
			}

			// Clean up
			StreamGrabber.FinishGrab();
			StreamGrabber.Close();
		}

		camera.Close();
		TlFactory.ReleaseTl(pTl);
	}
	catch (Pylon::GenericException &e)
	{
		// Error handling
		cerr << "An exception occurred!" << endl << e.GetDescription() << endl;
		return;
	}

	// Quit application
	return;
}
