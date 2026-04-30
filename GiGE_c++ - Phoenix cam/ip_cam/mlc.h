#pragma once

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>
#include<vector>
#include<Windows.h>
#include<chrono>
#include "grabber_g.h"
#include <time.h>
#include <direct.h>
#include <string>

#define num_leaf_pairs 30

using namespace std;

class SEC_FB
{



public:
	Grabber gigE;
	bool isGigEcam_init;
	BOOL gigEserverFound(char *acqServerName);
	BOOL ConstructorStatus = false;
	string serv_name;
	int x;
	int skip_n_frames;
	int cam_index;
	int wait;
	int do_sleep, cam_init_iter;
	bool  captureSuccess;
	/** constructor   */
	SEC_FB();
	/** destructor XOXO  */
	~SEC_FB();


	int status1;									//!< status of each caliberation stage..(success = 1; failure = 2) 
	int status2;
	int status3;
	int status4;
	bool display;									//!< toggle to ON/OFF display.
	bool display_field;
	string sec_fb_version = "V12.2.2.0.5"; // Added by Km to display in frontend

};
