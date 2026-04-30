#pragma once
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#   include <pylon/PylonGUI.h>
#endif

#include <pylon/BaslerUniversalInstantCameraArray.h>
#include <pylon/Info.h>
#include <pylon/gige/GigETransportLayer.h>
#include <pylon/gige/ActionTriggerConfiguration.h>
#include <pylon/gige/BaslerGigEDeviceInfo.h>
#include <pylon\gige\_BaslerGigECameraParams.h>
#include <pylon\gige\BaslerGigEInstantCamera.h>
#include <pylon/AcquireSingleFrameConfiguration.h>
#include <pylon\InstantCamera.h>
#include <pylon\InstantCameraArray.h>
#include <pylon\InstantInterface.h>
#include <pylon\_InstantCameraParams.h>
#include <pylon\_BaslerUniversalCameraParams.h>

#include <pylon\gige\BaslerGigECamera.h>

#include"Grabber.h"

#include <ostream>

using namespace Basler_UniversalCameraParams;
//using namespace Basler_GigECameraParams;


// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

class Basler_acer {


};