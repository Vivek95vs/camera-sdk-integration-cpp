#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"
#include <opencv2/opencv.hpp>
int main()
{

	IplImage * input;
	input = cvLoadImage("D:\vivek\My Doc\FINAL.jpg", 1);
	cvNamedWindow(“Output”, 1);
	cvShowImage(“Output”, input);
	cvWaitKey(0);
		 
}