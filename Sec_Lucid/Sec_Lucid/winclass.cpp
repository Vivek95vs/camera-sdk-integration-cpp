#include "stdafx.h"
#include "winclass.h"
#include "opencv2\highgui.hpp"

using namespace cv;


winclass::winclass()
{
	cv::namedWindow("Main", WINDOW_NORMAL);														/// strips
	setWindowProperty("Main", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	//imshow("Main", output); 
	waitKey(20);

}


winclass::~winclass()
{
}

void winclass::winhd()
{


}

