#include "iostream"
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
//#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudafilters.hpp" // cuda::Filter
#include "opencv2/cudaarithm.hpp"
#include <opencv2/core/cuda.hpp>

using namespace cv;


void scaleDownBrightness(cv::Mat& image, const cv::Mat& mask, double scale) {
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			if (mask.at<uchar>(y, x) == 255) {
				image.at<cv::Vec3b>(y, x) *= scale;
			}
		}
	}
}

// Function to replace scaled-down overexposed pixels with the median of neighboring pixels
void replaceScaledPixelsWithMedian(cv::Mat& image, const cv::Mat& mask, int kernelSize = 3) {
	cv::Mat result = image.clone();
	cv::Mat medianImage;
	cv::medianBlur(image, medianImage, kernelSize);

	for (int y = kernelSize / 2; y < image.rows - kernelSize / 2; ++y) {
		for (int x = kernelSize / 2; x < image.cols - kernelSize / 2; ++x) {
			if (mask.at<uchar>(y, x) == 255) {
				result.at<cv::Vec3b>(y, x) = medianImage.at<cv::Vec3b>(y, x);
			}
		}
	}

	image = result;
}

void replaceScaledPixelsWithRed(cv::Mat& image, const cv::Mat& mask) {
	cv::Vec3b red(0, 0, 255); // Red color in BGR format
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			if (mask.at<uchar>(y, x) == 255) {
				image.at<cv::Vec3b>(y, x) = red;
			}
		}
	}
}

void replaceSmallContoursWithRed(cv::Mat& image, const cv::Mat& mask, double maxContourArea) {
	// Find contours
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Create a mask for small contours
	cv::Mat smallContourMask = cv::Mat::zeros(mask.size(), CV_8UC1);
	for (const auto& contour : contours) {
		double area = cv::contourArea(contour);
		if (area < maxContourArea) {
			cv::drawContours(smallContourMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), cv::FILLED);
		}
	}

	// Replace small contour pixels with red
	cv::Vec3b red(0, 0, 255); // Red color in BGR format
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			if (smallContourMask.at<uchar>(y, x) == 255) {
				image.at<cv::Vec3b>(y, x) = red;
			}
		}
	}
}


int main()
{

	//Image Dilate for gap close

	//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Auto_log\\segmenttest.png");
	//if (image.empty()) {
	//	std::cerr << "Could not read the image" << std::endl;
	//	return 1;
	//}

	//int morph_size = 15; // Adjust the size to fit the gaps
	//cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
	//	cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
	//	cv::Point(morph_size, morph_size));
	//cv::Mat closedImage;
	//cv::morphologyEx(image, closedImage, cv::MORPH_CLOSE, element);

	//cv::imwrite("D:\\PMT_Radiotherapy\\Mlc\\Auto_log\\closedimage.png", closedImage);


	//Overexposure 

	//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png");
	//if (image.empty()) {
	//	std::cerr << "Error: Image not found" << std::endl;
	//	return -1;
	//}

	//// Convert to grayscale (if the image is in color)
	//cv::Mat grayImage;
	//if (image.channels() == 3) {
	//	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
	//}
	//else {
	//	grayImage = image; // Already grayscale
	//}

	//// Threshold to find overexposed areas
	//// Detect overexposed areas
	//int threshold = 240; // Threshold to identify overexposed areas
	//cv::Mat overexposedMask;
	//cv::threshold(grayImage, overexposedMask, threshold, 255, cv::THRESH_BINARY);

	//// Reduce overexposure by scaling down the pixel values
	//cv::Mat correctedImage = image.clone();
	//cv::Mat scaledImage;
	//cv::convertScaleAbs(image, scaledImage, 0.7); // Scale down by 0.5 (or adjust as needed)

	//											  // Apply the corrected image to overexposed areas
	//cv::bitwise_and(scaledImage, scaledImage, correctedImage, ~overexposedMask);
	//cv::bitwise_or(correctedImage, scaledImage, correctedImage, overexposedMask);

	//// Save or display the result
	//cv::imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\corrected_image.jpg", correctedImage);


	// Exposure reduced but exposed area are coming as black


	//Mat image = imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png");
	//if (image.empty()) {
	//	std::cerr << "Error loading image!" << std::endl;
	//	return -1;
	//}

	//// Convert to Lab color space
	//Mat labImage;
	//cvtColor(image, labImage, COLOR_BGR2Lab);

	//// Split into channels
	//std::vector<Mat> labChannels(3);
	//split(labImage, labChannels);
	//Mat L = labChannels[0]; // Luminance channel

	//						// Threshold to find overexposed areas
	//Mat mask;
	//threshold(L, mask, 220, 255, THRESH_BINARY); // Adjust threshold as needed

	//											 // Reduce exposure in overexposed areas
	//Mat reducedExposure;
	//L.copyTo(reducedExposure);
	//reducedExposure.setTo(0, mask); // Set overexposed areas to 0

	//								// Merge channels and convert back to BGR
	//labChannels[0] = reducedExposure;
	//Mat reducedLabImage;
	//merge(labChannels, reducedLabImage);
	//Mat result;
	//cvtColor(reducedLabImage, result, COLOR_Lab2BGR);

	//// Save the color image
	//cv::imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStichedmask.png", result);

	//======================================Overexposed area==============================

	// Load the image
	//Mat image = imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png");
	//if (image.empty()) {
	//	std::cerr << "Error loading image!" << std::endl;
	//	return -1;
	//}

	//// Convert to Lab color space
	//Mat labImage;
	//cvtColor(image, labImage, COLOR_BGR2Lab);

	//// Split into channels
	//std::vector<Mat> labChannels(3);
	//split(labImage, labChannels);
	//Mat L = labChannels[0]; // Luminance channel

	//						// Threshold to find overexposed areas
	//Mat mask;
	//threshold(L, mask, 220, 255, THRESH_BINARY); // Adjust threshold as needed

	//											 // Reduce exposure in overexposed areas
	//Mat reducedExposure;
	//L.copyTo(reducedExposure);
	//reducedExposure.setTo(0, mask); // Set overexposed areas to 0

	//								// Merge channels and convert back to BGR
	//labChannels[0] = reducedExposure;
	//Mat reducedLabImage;
	//merge(labChannels, reducedLabImage);
	//Mat reducedImage;
	//cvtColor(reducedLabImage, reducedImage, COLOR_Lab2BGR);

	//// Convert reduced image to grayscale for inpainting
	//Mat grayReducedImage;
	//cvtColor(reducedImage, grayReducedImage, COLOR_BGR2GRAY);

	//// Create a mask for black areas in the reduced image
	//Mat blackMask;
	//threshold(grayReducedImage, blackMask, 1, 255, THRESH_BINARY_INV); // Mask where black areas are white

	//																   // Find contours to identify small areas
	//std::vector<std::vector<Point>> contours;
	//std::vector<Vec4i> hierarchy;
	//findContours(blackMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//// Filter out small contours based on area
	//Mat smallAreaMask = Mat::zeros(blackMask.size(), CV_8UC1);
	//for (const auto& contour : contours) {
	//	double area = contourArea(contour);
	//	if (area < 300) { // Adjust the area threshold as needed
	//		drawContours(smallAreaMask, std::vector<std::vector<Point>>{contour}, -1, Scalar(255), FILLED);
	//	}
	//}

	//// Debug: Save the small area mask to verify
	//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\areamask.png", smallAreaMask);

	//// Inpaint only the small areas
	//Mat inpaintedImage;
	//inpaint(reducedImage, smallAreaMask, inpaintedImage, 3, INPAINT_TELEA); // Adjust radius and method as needed

	//																		// Save or display the final result
	//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStichedmask.png", inpaintedImage);

//gray pixel added

	//// Load the image
	//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png");
	//if (image.empty()) {
	//	std::cerr << "Could not open the image file." << std::endl;
	//	return -1;
	//}

	//// Convert to grayscale
	//cv::Mat gray;
	//cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	//// Thresholding to find overexposed areas
	//cv::Mat overexposed;
	//double thresholdValue = 240; // you can adjust this value
	//cv::threshold(gray, overexposed, thresholdValue, 255, cv::THRESH_BINARY);

	//// Create a mask of the overexposed areas
	//cv::Mat mask;
	//cv::bitwise_not(overexposed, mask);

	//// Reduce overexposure by scaling down the intensity in the overexposed areas
	//cv::Mat result = image.clone();
	//cv::Mat overexposedRegions;
	//image.copyTo(overexposedRegions, overexposed);

	//// Reduce intensity by scaling down
	//double scalingFactor = 0.5; // reduce to 50% of original intensity
	//overexposedRegions.convertTo(overexposedRegions, -1, scalingFactor, 0);

	//// Combine the reduced overexposed regions with the rest of the image
	//overexposedRegions.copyTo(result, overexposed);

	//// Display the results

	//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\overexposed.png", overexposed);
	//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\Reduced Overexposure.png", result);
//====================================================================

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png");
//if (image.empty()) {
//	std::cerr << "Error: Image not found" << std::endl;
//	return -1;
//}
//
//// Convert to grayscale (if the image is in color)
//cv::Mat grayImage;
//if (image.channels() == 3) {
//	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
//}
//else {
//	grayImage = image; // Already grayscale
//}
//
//// Threshold to find overexposed areas
//int threshold = 240; // Threshold to identify overexposed areas
//cv::Mat overexposedMask;
//cv::threshold(grayImage, overexposedMask, threshold, 255, cv::THRESH_BINARY);
//
//// Replace overexposed pixels with neighboring pixels
//cv::Mat result = image.clone();
//
//for (int y = 1; y < image.rows - 1; ++y) {
//	for (int x = 1; x < image.cols - 1; ++x) {
//		if (overexposedMask.at<uchar>(y, x) == 255) {
//			cv::Vec3b sum(0, 0, 0);
//			int count = 0;
//
//			// Sum the values of the 8-connected neighbors
//			for (int dy = -1; dy <= 1; ++dy) {
//				for (int dx = -1; dx <= 1; ++dx) {
//					if (dy != 0 || dx != 0) {
//						int ny = y + dy;
//						int nx = x + dx;
//						if (overexposedMask.at<uchar>(ny, nx) == 0) {
//							sum += image.at<cv::Vec3b>(ny, nx);
//							count++;
//						}
//					}
//				}
//			}
//
//			// Compute the average of the neighbors
//			if (count > 0) {
//				result.at<cv::Vec3b>(y, x) = sum / count;
//			}
//		}
//	}
//}
//
//image = result;
//
//// Save or display the result
//cv::imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\corrected_image.jpg", image);
//================================================================================================

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png");
//if (image.empty()) {
//	std::cerr << "Error: Image not found" << std::endl;
//	return -1;
//}
//
//// Convert to grayscale (if the image is in color)
//cv::Mat grayImage;
//if (image.channels() == 3) {
//	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
//}
//else {
//	grayImage = image; // Already grayscale
//}
//
//// Threshold to find overexposed areas
//int threshold = 240; // Threshold to identify overexposed areas
//cv::Mat overexposedMask;
//cv::threshold(grayImage, overexposedMask, threshold, 255, cv::THRESH_BINARY);
//
//// Scale down the brightness of overexposed areas
//scaleDownBrightness(image, overexposedMask, 0.7); // Scale down by 0.7
//
//												  // Replace scaled-down overexposed pixels with the median of neighboring pixels
////replaceScaledPixelsWithMedian(image, overexposedMask);
//
//// Replace scaled-down overexposed pixels with red
////replaceScaledPixelsWithRed(image, overexposedMask);
//
//replaceSmallContoursWithRed(image, overexposedMask, 300.0);
//
//
//// Save or display the result
//cv::imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\corrected_image.jpg", image);

//========================================================================================================

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\FinalStiched.png", cv::IMREAD_COLOR);
//
//// Check if the image is loaded
//if (image.empty()) {
//	std::cerr << "Could not open or find the image" << std::endl;
//	return -1;
//}
//
//// Convert the image to grayscale for easier thresholding
//cv::Mat gray;
//cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
//
//// Threshold to identify overexposed areas (values near 255)
//cv::Mat overexposedMask;
//cv::inRange(gray, cv::Scalar(200), cv::Scalar(255), overexposedMask);
//
//// Extract the overexposed region from the image using the mask
//cv::Mat extracted;
//image.copyTo(extracted, overexposedMask);
//
//// Convert mask to CV_32FC1 for mean calculation
//cv::Mat maskFloat;
//overexposedMask.convertTo(maskFloat, CV_32FC1, 1.0 / 255.0);
//
//// Compute the sum of pixel values weighted by the mask
//cv::Scalar sum = cv::sum(extracted);
//double count = cv::sum(maskFloat)[0];
//
//// Calculate the average pixel value
//std::cout << "Exposed area" << count;
//cv::Scalar avgPixelValue;
//if (count > 0) {
//	avgPixelValue[0] = sum[0] / count;
//	avgPixelValue[1] = sum[1] / count;
//	avgPixelValue[2] = sum[2] / count;
//}
//else {
//	avgPixelValue = cv::Scalar(0, 0, 0); // Default if no overexposed area is found
//}
//
//// Create an image with the average pixel value to replace the overexposed area
//cv::Mat averageImage(image.size(), image.type(), avgPixelValue);
//
//// Use the mask to replace the overexposed regions with the average pixel value
//cv::Mat correctedImage = image.clone();
//averageImage.copyTo(correctedImage, overexposedMask);
//
//
////replaceSmallContoursWithRed(image, overexposedMask, 300.0);
//
//
//// Save or display the result
//cv::imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\corrected_image.jpg", correctedImage);
//==============================================================================================
//====================Light Reflection Method===================================================

//Specular Reflection Removal

//Mat image = imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\Reflection checking\\Original Image.png");
//if (image.empty()) {
//	std::cout << "Could not open or find the image!" << std::endl;
//	return -1;
//}
//
//// Convert to grayscale
//Mat gray;
//cvtColor(image, gray, COLOR_BGR2GRAY);
//
//// Threshold to create a mask for bright spots
//Mat mask;
//threshold(gray, mask, 240, 255, THRESH_BINARY);
//
//// Morphological operations to clean up the mask
//Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
//morphologyEx(mask, mask, MORPH_CLOSE, kernel);
//morphologyEx(mask, mask, MORPH_OPEN, kernel);
//
//// Inpainting
//Mat result;
//inpaint(image, mask, result, 3, INPAINT_TELEA);
//
//// Save and display the result
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\Reflection checking\\corrected_image.jpg", result);
//========================================================================================================

// Detect Red color

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Image Pix 1.png");
//
//if (image.empty()) {
//	std::cerr << "Could not open or find the image." << std::endl;
//	return -1;
//}
//
//cv::Mat hsv_image;
//cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
//
//// Define the lower and upper bounds for red color in HSV
////cv::Scalar lower_red1(0, 50, 20);    // Adjusted lower range for darker reds
////cv::Scalar upper_red1(10, 255, 255);
////cv::Scalar lower_red2(160, 50, 20);  // Adjusted lower range for darker reds
////cv::Scalar upper_red2(180, 255, 255);
//
//cv::Scalar lower_red1(0, 50, 20);    // Adjusted lower range for darker reds
//cv::Scalar upper_red1(10, 255, 255);
//cv::Scalar lower_red2(160, 50, 20);  // Adjusted lower range for darker reds
//cv::Scalar upper_red2(180, 255, 255);
//
//
//// Create masks for red color
//cv::Mat mask1, mask2;
//cv::inRange(hsv_image, lower_red1, upper_red1, mask1);
//cv::inRange(hsv_image, lower_red2, upper_red2, mask2);
//
//// Combine the masks
//cv::Mat red_mask = mask1 | mask2;
//
//// Create an output image initialized to black
//cv::Mat output_image = cv::Mat::zeros(image.size(), CV_8UC1);
//
//// Set red areas to white in the output image
//output_image.setTo(255, red_mask);
//
//// Display the result
////cv::imshow("Red Detection", output_image);
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Red Detection.jpg", output_image);

//===============================================================================


//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Image Pix 1.png");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//int redMin = 75;    // Minimum red value (lowered to include more shades)
//int redMax = 255;   // Maximum red value
//int greenMax = 100; // Maximum green value
//int blueMax = 100;  // Maximum blue value
//
//					// Create a mask for red color
//cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
//
//for (int y = 0; y < image.rows; ++y) {
//	for (int x = 0; x < image.cols; ++x) {
//		cv::Vec3b color = image.at<cv::Vec3b>(y, x);
//		int red = color[2];   // OpenCV uses BGR format
//		int green = color[1];
//		int blue = color[0];
//
//		// Check if the pixel is a shade of red
//		if (red >= redMin && red <= redMax && green <= greenMax && blue <= blueMax) {
//			mask.at<uchar>(y, x) = 255; // Mark this pixel as red
//		}
//	}
//}
//
//// Create an output image with all pixels set to black
//cv::Mat outputImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
//
//// Set the red areas to white
//outputImage.setTo(cv::Scalar(255, 255, 255), mask);
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Red Detection.jpg", outputImage);

//==========================================================================================

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\yellow-2.png");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//
//cv::Mat hsv_image;
//cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
//
//// Define the lower and upper bounds for red color in HSV
////cv::Scalar lower_red1(0, 50, 20);    // Adjusted lower range for darker reds
////cv::Scalar upper_red1(10, 255, 255);
////cv::Scalar lower_red2(160, 50, 20);  // Adjusted lower range for darker reds
////cv::Scalar upper_red2(180, 255, 255);
//
////cv::Scalar lower_red1(0, 50, 20);    // Lower hue range for darker reds
////cv::Scalar upper_red1(10, 255, 255); // Upper hue range for darker reds
////cv::Scalar lower_red2(160, 50, 20);  // Lower hue range for brighter reds
////cv::Scalar upper_red2(180, 255, 255); // Upper hue range for brighter reds
//
//cv::Scalar lower_yellow(20, 50, 100); // Lower bound for yellow
//cv::Scalar upper_yellow(60, 255, 255); // Upper bound for yellow
//
//// Create masks for red color
//cv::Mat mask1, mask2;
//cv::inRange(hsv_image, lower_yellow, upper_yellow, mask1);
////cv::inRange(hsv_image, lower_red2, upper_red2, mask2);
//
//// Combine the masks
//cv::Mat red_mask = mask1 /*| mask2*/;
//
//// Create an output image initialized to black
//cv::Mat output_image = cv::Mat::zeros(image.size(), CV_8UC1);
//
//// Set red areas to white in the output image
//output_image.setTo(255, red_mask);
//
//// Display the result
////cv::imshow("Red Detection", output_image);
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Yekkow Detection.jpg", output_image);


//================================================================================================

//Red color detection

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Image Pix 1.png");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//
//cv::Mat hsv_image;
//cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
//
//// Define the lower and upper bounds for the yellow color
////cv::Scalar lower_yellow(20, 50, 100); // Lower bound for yellow
////cv::Scalar upper_yellow(60, 255, 255); // Upper bound for yellow
//
//									   // Create a mask for the yellow color
//cv::Scalar lower_red1(0, 50, 20);    // Adjusted lower range for darker reds
//cv::Scalar upper_red1(10, 255, 255);
//cv::Scalar lower_red2(160, 50, 20);  // Adjusted lower range for darker reds
//cv::Scalar upper_red2(180, 255, 255);
//
////cv::Mat yellow_mask;
////cv::inRange(hsv_image, lower_yellow, upper_yellow, yellow_mask);
//
//cv::Mat mask1, mask2;
//cv::inRange(hsv_image, lower_red1, upper_red1, mask1);
//cv::inRange(hsv_image, lower_red2, upper_red2, mask2);
//
//// Combine the masks
//cv::Mat red_mask = mask1 | mask2;
////
//
//// Create an output image initialized to black
//cv::Mat output_image = cv::Mat::zeros(image.size(), CV_8UC1);
//
//cv::Mat output_image1 = cv::Mat::zeros(image.size(), CV_8UC1);
//
//output_image.setTo(255, red_mask);
//
//// Define the size of the neighborhood
//int neighborhood_size = 35;
//int half_size = neighborhood_size / 2;
//
//// Process each pixel in the mask
//for (int i = 0; i < output_image.rows; i++) {
//	for (int j = 0; j < output_image.cols; j++) {
//		if (output_image.at<uchar>(i, j) == 255) {
//			int yellow_count = 0;
//			int total_count = 0;
//
//			// Check the neighborhood around the pixel
//			for (int x = -half_size; x <= half_size; x++) {
//				for (int y = -half_size; y <= half_size; y++) {
//					int nx = j + x;
//					int ny = i + y;
//
//					// Ensure pixel is within image boundaries
//					if (nx >= 0 && nx < output_image.cols && ny >= 0 && ny < output_image.rows) {
//						total_count++;
//						if (output_image.at<uchar>(ny, nx) == 255) {
//							yellow_count++;
//						}
//					}
//				}
//			}
//
//			// Determine if the pixel should be kept based on the percentage of yellow neighbors
//			if (yellow_count * 100.0 / total_count >= 50) { // Adjust the threshold as needed
//				output_image1.at<uchar>(i, j) = 255; // Keep the yellow pixel
//			}
//		}
//	}
//}
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Yellow Detection.jpg", output_image1);
//
//getchar();
//
//return 0;

//=======================================================================================

//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\FinalStiched.png");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//
//cv::Mat hsv_image;
//cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
//
// //Define the lower and upper bounds for the yellow color
//cv::Scalar lower_yellow_brown(0, 30, 50);  // Lower bound for yellow-brown
//cv::Scalar upper_yellow_brown(35, 255, 200); // Upper bound for yellow-brown
//
//
//cv::Mat yellow_mask;
//cv::inRange(hsv_image, lower_yellow_brown, upper_yellow_brown, yellow_mask);
//
//
// Create an output image initialized to black
//cv::Mat output_image = cv::Mat::zeros(image.size(), CV_8UC1);
//
//
// Define the size of the neighborhood
//int neighborhood_size = 1;
//int half_size = neighborhood_size / 2;
//
// Process each pixel in the mask
//for (int i = 0; i < yellow_mask.rows; i++) {
//	for (int j = 0; j < yellow_mask.cols; j++) {
//		if (yellow_mask.at<uchar>(i, j) == 255) {
//			int yellow_count = 0;
//			int total_count = 0;
//
//			 Check the neighborhood around the pixel
//			for (int x = -half_size; x <= half_size; x++) {
//				for (int y = -half_size; y <= half_size; y++) {
//					int nx = j + x;
//					int ny = i + y;
//
//					 Ensure pixel is within image boundaries
//					if (nx >= 0 && nx < yellow_mask.cols && ny >= 0 && ny < yellow_mask.rows) {
//						total_count++;
//						if (yellow_mask.at<uchar>(ny, nx) == 255) {
//							yellow_count++;
//						}
//					}
//				}
//			}
//
//			 Determine if the pixel should be kept based on the percentage of yellow neighbors
//			if (yellow_count * 100.0 / total_count >= 50) { // Adjust the threshold as needed
//				output_image.at<uchar>(i, j) = 255; // Keep the yellow pixel
//			}
//		}
//	}
//}
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Yellow Detection.jpg", output_image);
//
//getchar();
//
//return 0;



//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Image Pix 1.png");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//int redMin = 50;    // Minimum red value (lowered to include more shades)
//int redMax = 255;   // Maximum red value
//int greenMin = 0;   // Minimum green value
//int greenMax = 75;  // Maximum green value
//int blueMin = 0;    // Minimum blue value
//int blueMax = 75;   // Maximum blue value
//
//					// Create a mask for red color
//					// Create a mask for red color
//cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
//
//for (int y = 0; y < image.rows; ++y) {
//	for (int x = 0; x < image.cols; ++x) {
//		cv::Vec3b color = image.at<cv::Vec3b>(y, x);
//		int red = color[2];   // OpenCV uses BGR format
//		int green = color[1];
//		int blue = color[0];
//
//		// Check if the pixel is a shade of red
//		if (red >= redMin && red <= redMax && green >= greenMin && green <= greenMax && blue >= blueMin && blue <= blueMax) {
//			mask.at<uchar>(y, x) = 255; // Mark this pixel as red
//		}
//	}
//}
//
//// Create a result image with white areas for detected red and black for others
//cv::Mat resultImage = cv::Mat::zeros(image.size(), CV_8UC3);
//image.copyTo(resultImage, mask);
//
//// Convert resultImage to grayscale
//cv::Mat grayResult;
//cv::cvtColor(resultImage, grayResult, cv::COLOR_BGR2GRAY);
//
//// Threshold to make red areas white and others black
//cv::Mat binaryResult;
//cv::threshold(grayResult, binaryResult, 1, 255, cv::THRESH_BINARY);
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\RGB Value.jpg", binaryResult);
//
//getchar();
//
//return 0;

////////////////////////////with Cuda neighbour pixel



//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\FinalStiched.png");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//auto start = std::chrono::high_resolution_clock::now();
//
//// Upload image to GPU
//cv::cuda::GpuMat d_yellow_mask, d_hsv, imgThresholded, d_filtered;
//d_yellow_mask.upload(image);
//auto after_upload = std::chrono::high_resolution_clock::now();
//std::cout << "Upload time: " << std::chrono::duration_cast<std::chrono::milliseconds>(after_upload - start).count() << " ms\n";
//
//// Convert color space
//cv::cuda::cvtColor(d_yellow_mask, d_hsv, cv::COLOR_BGR2HSV);
//auto after_cvtColor = std::chrono::high_resolution_clock::now();
//std::cout << "Color conversion time: " << std::chrono::duration_cast<std::chrono::milliseconds>(after_cvtColor - after_upload).count() << " ms\n";
//
//// Perform thresholding
//cv::Scalar lower_yellow(0, 30, 50);
//cv::Scalar upper_yellow(35, 255, 255);
//cv::cuda::inRange(d_hsv, lower_yellow, upper_yellow, imgThresholded);
//auto after_threshold = std::chrono::high_resolution_clock::now();
//std::cout << "Thresholding time: " << std::chrono::duration_cast<std::chrono::milliseconds>(after_threshold - after_cvtColor).count() << " ms\n";
//
//auto total_processing_time = std::chrono::duration_cast<std::chrono::milliseconds>(after_threshold - start).count();
//std::cout << "Total processing time: " << total_processing_time << " ms\n";
//
//int neighborhood_size = 3; // Use 3x3 neighborhood
//int half_size = neighborhood_size / 2;
//
//// Create a CUDA box filter
//cv::Ptr<cv::cuda::Filter> boxFilter = cv::cuda::createBoxFilter(
//	d_yellow_mask.type(),  // Input type
//	d_yellow_mask.type(),  // Output type
//	cv::Size(neighborhood_size, neighborhood_size),  // Kernel size
//	cv::Point(-1, -1),     // Anchor point (default is center)
//	cv::BORDER_CONSTANT,   // Border type
//	cv::Scalar(0)          // Border value (for constant border type)
//);
//
//// Apply the filter
//boxFilter->apply(d_yellow_mask, d_filtered);
//
//// Initialize output image on GPU to zeros
//cv::cuda::GpuMat d_output_image(d_yellow_mask.size(), d_yellow_mask.type());
//d_output_image.setTo(cv::Scalar(0));
//
//// Calculate the threshold for the number of yellow pixels required
//int threshold = static_cast<int>(0.5 * neighborhood_size * neighborhood_size); // 50% of the neighborhood
//
//																			   // Apply threshold to determine which pixels should be kept as yellow
//cv::cuda::threshold(d_filtered, d_output_image, threshold * 255 / 9, 255, cv::THRESH_BINARY);
//
//// Download the result from GPU to CPU
//cv::Mat output_image;
//d_output_image.download(output_image);
//
//auto start12 = std::chrono::high_resolution_clock::now();
//cv::Mat origi;
//imgThresholded.download(origi);
//auto after_upload12 = std::chrono::high_resolution_clock::now();
//std::cout << "download time: " << std::chrono::duration_cast<std::chrono::milliseconds>(after_upload12 - start12).count() << " ms\n";
//
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Yellow Detection.jpg", origi);
//
//getchar();
//
//return 0;



//cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\Auto_log\\White balance Auto\\Bank B.jpg");
//
//if (image.empty()) {
//	std::cerr << "Error: Image not found!" << std::endl;
//	return -1;
//}
//
//// Split the image into B, G, R channels
//std::vector<cv::Mat> channels;
//cv::split(image, channels);
//
//// Calculate the mean for each channel
//cv::Scalar meanB = cv::mean(channels[0]);
//cv::Scalar meanG = cv::mean(channels[1]);
//cv::Scalar meanR = cv::mean(channels[2]);
//
//// Output the mean values
//std::cout << "Mean Blue: " << meanB[0] << std::endl;
//std::cout << "Mean Green: " << meanG[0] << std::endl;
//std::cout << "Mean Red: " << meanR[0] << std::endl;
//
//// Check white balance
//double threshold = 10.0; // Adjust this threshold value as needed
//if (std::abs(meanB[0] - meanG[0]) < threshold && std::abs(meanB[0] - meanR[0]) < threshold && std::abs(meanG[0] - meanR[0]) < threshold) {
//	std::cout << "White balance is good." << std::endl;
//}
//else {
//	std::cout << "White balance needs adjustment." << std::endl;
//}

//cv::cuda::GpuMat imgOriginal, d_hsv, imgThresholded;
//imgOriginal.upload(image);
//
//cv::cuda::cvtColor(imgOriginal, d_hsv, cv::COLOR_BGR2HSV);
//
//int nearpixel1 = 30;
//
//// Define the lower and upper bounds for the yellow color
//cv::Scalar lower_yellow(0, 30, 50);  // Lower bound for yellow
//cv::Scalar upper_yellow(35, 255, 255);  // Upper bound for yellow
//
//										// Perform thresholding to find yellow pixels
//cv::cuda::inRange(d_hsv, lower_yellow, upper_yellow, imgThresholded);
//
//auto after_threshold12 = std::chrono::high_resolution_clock::now();
//
//cv::Ptr<cv::cuda::Filter> boxFilter = cv::cuda::createBoxFilter(
//	imgThresholded.type(),  // Input type
//	imgThresholded.type(),  // Output type
//	cv::Size(nearpixel1, nearpixel1),  // Kernel size
//	cv::Point(-1, -1),     // Anchor point (default is center)
//	cv::BORDER_CONSTANT,   // Border type
//	cv::Scalar(0)          // Border value (for constant border type)
//);
//
//// Apply the filter
//boxFilter->apply(imgThresholded, imgThresholded);
//
//cv::Mat origi;
//imgThresholded.download(origi);
//auto after_upload12 = std::chrono::high_resolution_clock::now();
//
//imwrite("D:\\PMT_Radiotherapy\\Mlc\\Exposure image\\issue\\Yellow Detection123.jpg", origi);

cv::Mat image = cv::imread("D:\\PMT_Radiotherapy\\Mlc\\issue-image\\original image.png");

if (image.empty()) {
	std::cerr << "Error: Image not found!" << std::endl;
	return -1;
}

cv::Size morph_size = cv::Size(6, 6);
cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, morph_size);
cv::Ptr<cv::cuda::Filter> dilateFilter1 = cuda::createMorphologyFilter(MORPH_DILATE, CV_8UC1, element);
cv::Ptr<cv::cuda::Filter> erodeFilter1 = cuda::createMorphologyFilter(MORPH_ERODE, CV_8UC1, element);

cv::cuda::GpuMat imgOriginal, d_hsv, imgThresholded, d_filtered;
imgOriginal.upload(image);

cv::Mat out;

// Convert BGR to HSV using CUDA
cv::cuda::cvtColor(imgOriginal, d_hsv, cv::COLOR_BGR2HSV);

// Define the lower and upper bounds for the yellow color
cv::Scalar lower_yellow(0, 30, 50);  // Lower bound for yellow
cv::Scalar upper_yellow(35, 255, 255);  // Upper bound for yellow

int nearpixel1 = 1;

										// Perform thresholding to find yellow pixels
cv::cuda::inRange(d_hsv, lower_yellow, upper_yellow, imgThresholded);

auto after_threshold12 = std::chrono::high_resolution_clock::now();

cv::Ptr<cv::cuda::Filter> boxFilter = cv::cuda::createBoxFilter(
	imgThresholded.type(),  // Input type
	imgThresholded.type(),  // Output type
	cv::Size(nearpixel1, nearpixel1),  // Kernel size
	cv::Point(-1, -1),     // Anchor point (default is center)
	cv::BORDER_CONSTANT,   // Border type
	cv::Scalar(0)          // Border value (for constant border type)
);

// Apply the filter
boxFilter->apply(imgThresholded, d_filtered);

cv::cuda::GpuMat d_output_image(d_filtered.size(), CV_8UC1);
d_output_image.setTo(cv::Scalar(0));

// Calculate the threshold for the number of yellow pixels required
int threshold = static_cast<int>(0.5 * nearpixel1 * nearpixel1); // 50% of the neighborhood

																 // Apply threshold to determine which pixels should be kept as yellow
cv::cuda::threshold(d_filtered, d_output_image, threshold, 255, cv::THRESH_BINARY);
imgThresholded.release();
d_output_image.copyTo(imgThresholded);


int dilate1 = 3;
int erode1 = 0;
int open1 = 0;
int close1 = 3;


if (dilate1)
{// 13-03-2018
	for (int i = 0; i < dilate1; i++)
		dilateFilter1->apply(imgThresholded, imgThresholded);

}
if (erode1)
{
	for (int i = 0; i < erode1; i++)
		erodeFilter1->apply(imgThresholded, imgThresholded);
}
if (open1)
{
	for (int i = 0; i < open1; i++)
		erodeFilter1->apply(imgThresholded, imgThresholded);		// OPEN
	for (int i = 0; i < open1; i++)
		dilateFilter1->apply(imgThresholded, imgThresholded);
}
if (close1)
{
	for (int i = 0; i < close1; i++)
		dilateFilter1->apply(imgThresholded, imgThresholded);			// CLOSE
																		//std::cout << "Time for Morph: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_Morph).count() << " milli_sec\n" << endl;
	for (int i = 0; i < close1; i++)
		erodeFilter1->apply(imgThresholded, imgThresholded);
}

imgThresholded.download(out);
imwrite("D:\\PMT_Radiotherapy\\Mlc\\issue-image\\Yellow Detection.jpg", out);



getchar();

return 0;

}

					// Callback function for the trackbar
