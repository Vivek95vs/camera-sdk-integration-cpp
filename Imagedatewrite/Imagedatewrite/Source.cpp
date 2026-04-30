#include <opencv2/opencv.hpp>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS

int main()
{
	cv::Mat image = cv::imread("D:\\Bhabhatron_image_log\\Invalid_image.png");  // read image file
	time_t now = time(0);
	struct tm tstruct;
	char datetime[80];
	tstruct = *localtime(&now);
	strftime(datetime, sizeof(datetime), "%Y-%m-%d_%H-%M-%S", &tstruct);  // format date and time as a string
	std::string filename = "D:\\Bhabhatron_image_log\\Invalid_image" + std::string(datetime) + ".png";  // add formatted date and time to filename
	cv::imwrite(filename, image);


}