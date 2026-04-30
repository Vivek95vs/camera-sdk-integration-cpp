

#include "mlc.h"

#define CORSERVER_MAX_STRLEN		30
SEC_FB::SEC_FB()
{
	isGigEcam_init = false;							/// restore_track-> if set to 1, track values stored in file is made as current track bar values.

	gigE.startsystem();

	char acqServerName[CORSERVER_MAX_STRLEN] = "";
	UINT32   acqDeviceNumber = 0;

	if (gigEserverFound(acqServerName)) {

		//gigE = Grabber(acqServerName, NULL);


		while (1)
		{
		 gigE.grabFrame();

		}


	}
	else {
		cout << "No server found" << endl;
	}
}
SEC_FB::~SEC_FB()
{

}





BOOL SEC_FB::gigEserverFound(char *acqServerName)
{
	//char *acqServerName; /*UINT32 *pAcqDeviceIndex;BOOL showGigEOnly = FALSE;*/ 
	int camera_finder_iter = 0;

	/*std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();*/
AGAIN:

	int serverCount = gigE.deviceInfos.size();
	int acqDeviceCount = 0; int deviceCount = 0;

	int GenieIndex = 0; int deviceIndex = 0;

	std::vector<std::string> v_ServerNames;
	if (serverCount == 0)
	{
		printf("No device found!\n");
		return FALSE;
	}

#ifdef GRAB_CAMERA_LINK
	printf("\nNote:\nOnly CameraLink cameras will work with this example !\nBehavior is undefined for any other devices. \n");
#endif

	printf("\n camera(s) detected ");
	printf("\n....................................................\n");

	std::string serverName;
	BOOL serverFound = FALSE;

	for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
	{
		serverName = gigE.deviceInfos[0].ModelName();

		printf("%d: %s\n", GenieIndex + 1, serverName);
		GenieIndex++;
		serverFound = TRUE;
		deviceCount = GenieIndex;

		std::string deviceName;
		deviceIndex = 0;
		//SapManager::GetResourceName(serverName, SapManager::ResourceAcqDevice, deviceIndex, deviceName, sizeof(deviceName));
		deviceName = gigE.deviceInfos[0].UserDefinedName();
		printf("    %s%s\n", "User defined Name : ", deviceName);
		printf("........................................\n");
		v_ServerNames.push_back(serverName);
	}

	// At least one acquisition server must be available
	if ((!serverFound) && (camera_finder_iter < 10))
	{
		printf("No camera found!\n");

		//cv::waitKey(500);
		goto AGAIN;

	}
	else if ((!serverFound) && (camera_finder_iter >= 10)) {
		isGigEcam_init = false;
		return FALSE;
	}
	else {
		//_________________________________________________________________________
		isGigEcam_init = true;
		std::string sServerName = v_ServerNames[0];
		strncpy(acqServerName, sServerName.c_str(), sServerName.size() + 1);

		//_______________________________________
		printf("\n");
		return TRUE;
	}
}
