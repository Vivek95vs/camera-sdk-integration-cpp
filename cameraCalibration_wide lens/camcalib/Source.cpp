#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <conio.h>


#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif
using namespace cv;
using namespace std;

#define PI 3.1415926536
int viewcount = 0;

static void help()
{
	cout << "This is a camera calibration sample." << endl
		<< "Usage: calibration configurationFile" << endl
		<< "Near the sample file you'll find the configuration file, which has detailed help of "
		"how to edit it.  It may be any OpenCV supported file format XML/YAML." << endl;
}
class Settings
{
public:
	Settings() : goodInput(false) {}
	enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
	enum InputType { INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST };

	void write(FileStorage& fs) const                        //Write serialization for this class
	{
		fs << "{"
			<< "BoardSize_Width" << boardSize.width
			<< "BoardSize_Height" << boardSize.height
			<< "Square_Size" << squareSize
			<< "Calibrate_Pattern" << patternToUse
			<< "Calibrate_NrOfFrameToUse" << nrFrames
			<< "Calibrate_FixAspectRatio" << aspectRatio
			<< "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
			<< "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint

			<< "Write_DetectedFeaturePoints" << writePoints
			<< "Write_extrinsicParameters" << writeExtrinsics
			<< "Write_gridPoints" << writeGrid
			<< "Write_outputFileName" << outputFileName

			<< "Show_UndistortedImage" << showUndistorted

			<< "Input_FlipAroundHorizontalAxis" << flipVertical
			<< "Input_Delay" << delay
			<< "Input" << input
			<< "}";
	}
	void read(const FileNode& node)                          //Read serialization for this class
	{
		node["BoardSize_Width"] >> boardSize.width;
		node["BoardSize_Height"] >> boardSize.height;
		node["Calibrate_Pattern"] >> patternToUse;
		node["Square_Size"] >> squareSize;
		node["Calibrate_NrOfFrameToUse"] >> nrFrames;
		node["Calibrate_FixAspectRatio"] >> aspectRatio;
		node["Write_DetectedFeaturePoints"] >> writePoints;
		node["Write_extrinsicParameters"] >> writeExtrinsics;
		node["Write_gridPoints"] >> writeGrid;
		node["Write_outputFileName"] >> outputFileName;
		node["Calibrate_AssumeZeroTangentialDistortion"] >> calibZeroTangentDist;
		node["Calibrate_FixPrincipalPointAtTheCenter"] >> calibFixPrincipalPoint;
		node["Calibrate_UseFisheyeModel"] >> useFisheye;
		node["Input_FlipAroundHorizontalAxis"] >> flipVertical;
		node["Show_UndistortedImage"] >> showUndistorted;
		node["Input"] >> input;
		node["Input_Delay"] >> delay;
		node["Fix_K1"] >> fixK1;
		node["Fix_K2"] >> fixK2;
		node["Fix_K3"] >> fixK3;
		node["Fix_K4"] >> fixK4;
		node["Fix_K5"] >> fixK5;

		validate();
	}
	void validate()
	{
		goodInput = true;
		if (boardSize.width <= 0 || boardSize.height <= 0)
		{
			cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
			goodInput = false;
		}
		if (squareSize <= 10e-6)
		{
			cerr << "Invalid square size " << squareSize << endl;
			goodInput = false;
		}
		if (nrFrames <= 0)
		{
			cerr << "Invalid number of frames " << nrFrames << endl;
			goodInput = false;
		}

		if (input.empty())      // Check for valid input
			inputType = INVALID;
		else
		{
			if (input[0] >= '0' && input[0] <= '9')
			{
				stringstream ss(input);
				ss >> cameraID;
				inputType = CAMERA;
			}
			else
			{
				if (isListOfImages(input) && readStringList(input, imageList))
				{
					inputType = IMAGE_LIST;
					nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
				}
				else
					inputType = VIDEO_FILE;
			}
			if (inputType == CAMERA)
			{
				inputCapture.open(cameraID);
				cout << "cameraID = " << cameraID << endl;


				int img_wd_req;							//!<  colomns requested to camera
				int img_ht_req;                         //!<  rows requested to camera
				cv::FileStorage fsa("cam_mlc_props.xml", cv::FileStorage::READ);
				fsa["img_wd_req"] >> img_wd_req; //caqmera width requested
				fsa["img_ht_req"] >> img_ht_req;//camera height requested
				fsa.release();
				inputCapture.set(cv::CAP_PROP_FRAME_WIDTH, img_wd_req);	/// setting camera property (not necesary that camera will provide the requested resolution!)
				inputCapture.set(cv::CAP_PROP_FRAME_HEIGHT, img_ht_req);	/// camera might provide <= requested resolution. Better try to set res that camera can actually provide
				cout << "img_wd_req" << img_wd_req;
				cout << "img_ht_req" << img_ht_req;

				if (!inputCapture.isOpened())
					cout << "!inputCapture.isOpened()" << endl;
			}
			if (inputType == VIDEO_FILE)
				inputCapture.open(input);
			if (inputType != IMAGE_LIST && !inputCapture.isOpened())
				inputType = INVALID;
		}
		if (inputType == INVALID)
		{
			cerr << " Input does not exist: " << input;
			goodInput = false;
		}

		flag = 0;
		if (calibFixPrincipalPoint) flag |= CALIB_FIX_PRINCIPAL_POINT;
		if (calibZeroTangentDist)   flag |= CALIB_ZERO_TANGENT_DIST;
		if (aspectRatio)            flag |= CALIB_FIX_ASPECT_RATIO;
		if (fixK1)                  flag |= CALIB_FIX_K1;
		if (fixK2)                  flag |= CALIB_FIX_K2;
		if (fixK3)                  flag |= CALIB_FIX_K3;
		if (fixK4)                  flag |= CALIB_FIX_K4;
		if (fixK5)                  flag |= CALIB_FIX_K5;

		if (useFisheye) {
			// the fisheye model has its own enum, so overwrite the flags
			flag = fisheye::CALIB_FIX_SKEW | fisheye::CALIB_RECOMPUTE_EXTRINSIC;
			if (fixK1)                   flag |= fisheye::CALIB_FIX_K1;
			if (fixK2)                   flag |= fisheye::CALIB_FIX_K2;
			if (fixK3)                   flag |= fisheye::CALIB_FIX_K3;
			if (fixK4)                   flag |= fisheye::CALIB_FIX_K4;
			if (calibFixPrincipalPoint) flag |= fisheye::CALIB_FIX_PRINCIPAL_POINT;
		}

		calibrationPattern = NOT_EXISTING;
		if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
		if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
		if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
		if (calibrationPattern == NOT_EXISTING)
		{
			cerr << " Camera calibration mode does not exist: " << patternToUse << endl;
			goodInput = false;
		}
		atImageList = 0;

	}
	Mat nextImage()
	{
		Mat result;

		if (inputCapture.isOpened())
		{
			Mat view0;
			inputCapture >> view0;
			view0.copyTo(result);
		}
		else if (atImageList < imageList.size())
			result = imread(imageList[atImageList++], IMREAD_COLOR);

		return result;
	}

	static bool readStringList(const string& filename, vector<string>& l)
	{
		l.clear();
		FileStorage fs(filename, FileStorage::READ);
		if (!fs.isOpened())
			return false;
		FileNode n = fs.getFirstTopLevelNode();
		if (n.type() != FileNode::SEQ)
			return false;
		FileNodeIterator it = n.begin(), it_end = n.end();
		for (; it != it_end; ++it)
			l.push_back((string)*it);
		return true;
	}

	static bool isListOfImages(const string& filename)
	{
		string s(filename);
		// Look for file extension
		if (s.find(".xml") == string::npos && s.find(".yaml") == string::npos && s.find(".yml") == string::npos)
			return false;
		else
			return true;
	}
public:
	Size boardSize;              // The size of the board -> Number of items by width and height
	Pattern calibrationPattern;  // One of the Chessboard, circles, or asymmetric circle pattern
	float squareSize;            // The size of a square in your defined unit (point, millimeter,etc).
	int nrFrames;                // The number of frames to use from the input for calibration
	float aspectRatio;           // The aspect ratio
	int delay;                   // In case of a video input
	bool writePoints;            // Write detected feature points
	bool writeExtrinsics;        // Write extrinsic parameters
	bool writeGrid;              // Write refined 3D target grid points
	bool calibZeroTangentDist;   // Assume zero tangential distortion
	bool calibFixPrincipalPoint; // Fix the principal point at the center
	bool flipVertical;           // Flip the captured images around the horizontal axis
	string outputFileName;       // The name of the file where to write
	bool showUndistorted;        // Show undistorted images after calibration
	string input;                // The input ->
	bool useFisheye;             // use fisheye camera model for calibration
	bool fixK1;                  // fix K1 distortion coefficient
	bool fixK2;                  // fix K2 distortion coefficient
	bool fixK3;                  // fix K3 distortion coefficient
	bool fixK4;                  // fix K4 distortion coefficient
	bool fixK5;                  // fix K5 distortion coefficient

	int cameraID;
	vector<string> imageList;
	size_t atImageList;
	VideoCapture inputCapture;
	InputType inputType;
	bool goodInput;
	int flag;

private:
	string patternToUse;


};

static inline void read(const FileNode& node, Settings& x, const Settings& default_value = Settings())
{
	if (node.empty())
		x = default_value;
	else
		x.read(node);
}

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
	vector<vector<Point2f> > imagePoints, float grid_width, bool release_object);

//Point2f getInputPoint(int x, int y, int srcwidth, int srcheight)
//{
//	Point2f pfish;
//	float theta, phi, r, r2;
//	Point3f psph;
//	float FOV = (float)PI / 180 * 180;
//	float FOV2 = (float)PI / 180 * 180;
//	float width = srcwidth;
//	float height = srcheight;
//
//	// Polar angles
//	theta = PI * (x / width - 0.5); // -pi/2 to pi/2
//	phi = PI * (y / height - 0.5);  // -pi/2 to pi/2
//
//									// Vector in 3D space
//	psph.x = cos(phi) * sin(theta);
//	psph.y = cos(phi) * cos(theta);
//	psph.z = sin(phi) * cos(theta);
//
//	// Calculate fisheye angle and radius
//	theta = atan2(psph.z, psph.x);
//	phi = atan2(sqrt(psph.x*psph.x + psph.z*psph.z), psph.y);
//
//	r = width * phi / FOV;
//	r2 = height * phi / FOV2;
//
//	// Pixel in fisheye space
//	pfish.x = 0.5 * width + r * cos(theta);
//	pfish.y = 0.5 * height + r2 * sin(theta);
//	return pfish;
//}
int main()
{

	help();
	int argc = 2;
	char* argv[2];
	argv[1] = "in_VID5.xml";

	//! [file_read]
	Settings s;


	const string inputSettingsFile = argc > 1 ? argv[1] : "default.xml";
	FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the setting
	if (!fs.isOpened())
	{
		cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
		//parser.printMessage();
		return -1;
	}
	fs["Settings"] >> s;
	fs.release();                                         // close Settings file
														  //! [file_read]

														  //FileStorage fout("settings.yml", FileStorage::WRITE); // write config as YAML
														  //fout << "Settings" << s;

	if (!s.goodInput)
	{
		cout << "Invalid input detected. Application stopping. " << endl;
		getch();
		return -1;
	}

	//int winSize = parser.get<int>("winSize");

	float grid_width = s.squareSize * (s.boardSize.width - 1);
	bool release_object = true;
	//if (parser.has("d")) {
	//	grid_width = parser.get<float>("d");
	//	release_object = true;
	//}

	vector<vector<Point2f> > imagePoints;
	Mat cameraMatrix, distCoeffs;
	Size imageSize;
	int mode = s.inputType == Settings::IMAGE_LIST ? CAPTURING : DETECTION;
	clock_t prevTimestamp = 0;
	const Scalar RED(0, 0, 255), GREEN(0, 255, 0);
	const char ESC_KEY = 27;

	//! [get_input]
	for (;;)
	{
		Mat view;
		bool blinkOutput = false;

		view = s.nextImage();

		//view.size() = cv::Size(1536, 2048);
	

		cout << "View size" << view.size();

		//imageSize = view.size();
		//cout << "Size of view " << imageSize << endl;
		//-----  If no more image, or got enough, then stop calibration and show result -------------
		if (mode == CAPTURING && imagePoints.size() >= (size_t)s.nrFrames)
		{
			cout << "Entering capture mode" << endl;

			if (runCalibrationAndSave(s, imageSize, cameraMatrix, distCoeffs, imagePoints, grid_width,
				release_object))
				mode = CALIBRATED;
			else
				mode = DETECTION;
		}
		if (view.empty())          // If there are no more images stop the loop
		{
			cout << "Entering View mode" << endl;
			// if calibration threshold was not reached yet, calibrate now
			if (mode != CALIBRATED && !imagePoints.empty())
				runCalibrationAndSave(s, imageSize, cameraMatrix, distCoeffs, imagePoints, grid_width,
					release_object);
			break;
		}
		//! [get_input]

		imageSize = view.size();  // Format input image.



		cout << "Size of view " << imageSize<<endl;


		if (s.flipVertical)    flip(view, view, 0);

		//! [find_pattern]
		vector<Point2f> pointBuf;

		bool found;

		int chessBoardFlags = CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE;

		if (!s.useFisheye) {
			// fast check erroneously fails with high distortions like fisheye
			chessBoardFlags |= CALIB_CB_FAST_CHECK;
		}

		switch (s.calibrationPattern) // Find feature points on the input format
		{
		case Settings::CHESSBOARD:
			found = findChessboardCorners(view, s.boardSize, pointBuf, chessBoardFlags);
			break;
		case Settings::CIRCLES_GRID:
			found = findCirclesGrid(view, s.boardSize, pointBuf);
			break;
		case Settings::ASYMMETRIC_CIRCLES_GRID:
			found = findCirclesGrid(view, s.boardSize, pointBuf, CALIB_CB_ASYMMETRIC_GRID);
			break;
		default:
			found = false;
			break;
		}
		//! [find_pattern]
		//! [pattern_found]
		if (found)                // If done with success,
		{
			// improve the found corners' coordinate accuracy for chessboard
			if (s.calibrationPattern == Settings::CHESSBOARD)
			{
				Mat viewGray;
				cvtColor(view, viewGray, COLOR_BGR2GRAY);
				cornerSubPix(viewGray, pointBuf, /*Size(11, 11)*/Size(11, 11),
					Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.0001));
			}

			if (mode == CAPTURING &&  // For camera only take new samples after delay time
				(!s.inputCapture.isOpened() || clock() - prevTimestamp > s.delay*1e-3*CLOCKS_PER_SEC))
			{
				imagePoints.push_back(pointBuf);
				prevTimestamp = clock();
				blinkOutput = s.inputCapture.isOpened();
			}

			// Draw the corners.
			drawChessboardCorners(view, s.boardSize, Mat(pointBuf), found);
		}
		//! [pattern_found]
		//----------------------------- Output Text ------------------------------------------------
		//! [output_text]
		string msg = (mode == CAPTURING) ? "100/100" :
			mode == CALIBRATED ? "Calibrated" : "Press 'g' to start";
		int baseLine = 0;
		Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
		Point textOrigin(view.cols - 2 * textSize.width - 10, view.rows - 2 * baseLine - 10);

		if (mode == CAPTURING)
		{
			if (s.showUndistorted)
				msg = cv::format("%d/%d Undist", (int)imagePoints.size(), s.nrFrames);
			else
				msg = cv::format("%d/%d", (int)imagePoints.size(), s.nrFrames);
		}

		putText(view, msg, textOrigin, 1, 1, mode == CALIBRATED ? GREEN : RED);

		if (blinkOutput)
			bitwise_not(view, view);
		//! [output_text]
		//------------------------- Video capture  output  undistorted ------------------------------
		//! [output_undistorted]
		if (mode == CALIBRATED && s.showUndistorted)
		{
			Mat temp = view.clone();
			if (s.useFisheye)
			{
				Mat newCamMat;
				fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, distCoeffs, imageSize,
					Matx33d::eye(), newCamMat, 1);
				cv::fisheye::undistortImage(temp, view, cameraMatrix, distCoeffs, newCamMat);


			}
			else
				undistort(temp, view, cameraMatrix, distCoeffs);
		}
		//! [output_undistorted]
		//------------------------------ Show image and check for input commands -------------------
		//! [await_input]
	
		imshow("Image Calibration", view);
		char key = (char)waitKey(s.inputCapture.isOpened() ? 50 : s.delay);

		if (key == ESC_KEY)
			break;

		if (key == 'u' && mode == CALIBRATED)
			s.showUndistorted = !s.showUndistorted;

		if (s.inputCapture.isOpened() && key == 'g')
		{
			mode = CAPTURING;
			imagePoints.clear();
		}
		//! [await_input]
	}



	// -----------------------Show the undistorted image for the image list ------------------------
	//! [show_results]
	if (s.inputType == Settings::IMAGE_LIST && s.showUndistorted && !cameraMatrix.empty())
	{
		Mat view, rview, map1, map2, warpview, imageresize;

		//cv::Mat K(3, 3, cv::DataType<double>::type);

		//cv::Mat E = cv::Mat::eye(3, 3, cv::DataType<double>::type);
		//K.at<double>(0, 0) = 1307.2807020496643;
		//K.at<double>(0, 1) = 0.0;
		//K.at<double>(0, 2) = 530.3754311563506;

		//K.at<double>(1, 0) = 0.0;
		//K.at<double>(1, 1) = 1318.342691460933;
		//K.at<double>(1, 2) = 354.98352268131123;

		//K.at<double>(2, 0) = 0.0;
		//K.at<double>(2, 1) = 0.0;
		//K.at<double>(2, 2) = 1.0;

		//cv::Mat D(4, 1, cv::DataType<double>::type);
		//D.at<double>(0, 0) = -0.2994762856767568;
		//D.at<double>(1, 0) = 0.5036082961388784;
		//D.at<double>(2, 0) = -4.231072729639434;
		//D.at<double>(3, 0) = 3.8646397788794578;

		if (s.useFisheye)
		{
			Mat newCamMat;
			fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, distCoeffs, imageSize,
				Matx33d::eye(), newCamMat, 1);

			Mat newCamMat1 = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 5, imageSize, 0);

			fisheye::initUndistortRectifyMap(cameraMatrix, distCoeffs, Matx33d::eye(), /*getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0)*/newCamMat1, imageSize,
				CV_32FC1, map1, map2);

			cout << "Size of Image" << " " << imageSize << endl;
		}
		else
		{
			

			initUndistortRectifyMap(
				cameraMatrix, distCoeffs, Mat(),
				getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize,
				CV_32FC1, map1, map2);
		}

		for (size_t i = 0; i < s.imageList.size(); i++)
		{
			view = imread(s.imageList[i], IMREAD_COLOR);
			if (view.empty())
				continue;

			Mat new_camera_matrix = cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0);

			cv::undistort(view, rview, new_camera_matrix, distCoeffs, new_camera_matrix);

			std::string s = "D:\\Bhabhatron_gigE_calib\\Calibrated Image\\Undistorted_Test_View.png" ;
			//cv::resize(rview, imageresize, cv::Size(4000, 4000), 0, 0, cv::INTER_LINEAR);
			cv::imwrite(s, view);


			remap(view, rview, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
			std::string s1 = "D:\\Bhabhatron_gigE_calib\\Calibrated Image\\Undistorted_Test_Rview.png";
			cv::imwrite(s1, rview);

			
			//Mat pview(view.rows, view.cols, CV_8UC3);

			//for (int i = 0; i<pview.cols; i++)
			//{
			//	for (int j = 0; j<pview.rows; j++)
			//	{

			//		Point2f inP = getInputPoint(i, j, view.cols, view.rows);
			//		Point inP2((int)inP.x, (int)inP.y);

			//		if (inP2.x >= view.cols || inP2.y >= view.rows)
			//			continue;

			//		if (inP2.x < 0 || inP2.y < 0)
			//			continue;
			//		Vec3b color = view.at<Vec3b>(inP2);
			//		pview.at<Vec3b>(Point(i, j)) = color;

			//	}
			//}
			//Mat M = getPerspectiveTransform(src_vertices, dst_vertices);
			//warpPerspective(rview, warpview, M,rview.size());
			std::string s2 = "D:\\Bhabhatron_gigE_calib\\Calibrated Image\\Undistorted_" + std::to_string(viewcount) + ".png";
			//cv::resize(rview, imageresize, cv::Size(4000, 4000), 0, 0, cv::INTER_LINEAR);
			cv::imwrite(s2, rview);
			imshow("Image View", rview);	
			viewcount++;
			char c = (char)waitKey();
			if (c == ESC_KEY || c == 'q' || c == 'Q')
				break;
		}
	}
	//! [show_results]

	return 0;
}

//! [compute_errors]
static double computeReprojectionErrors(const vector<vector<Point3f> >& objectPoints,
	const vector<vector<Point2f> >& imagePoints,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const Mat& cameraMatrix, const Mat& distCoeffs,
	vector<float>& perViewErrors, bool fisheye)
{
	vector<Point2f> imagePoints2;
	size_t totalPoints = 0;
	double totalErr = 0, err;
	perViewErrors.resize(objectPoints.size());

	for (size_t i = 0; i < objectPoints.size(); ++i)
	{
		if (fisheye)
		{
			fisheye::projectPoints(objectPoints[i], imagePoints2, rvecs[i], tvecs[i], cameraMatrix,
				distCoeffs);
		}
		else
		{
			projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
		}
		err = norm(imagePoints[i], imagePoints2, NORM_L2);

		size_t n = objectPoints[i].size();
		perViewErrors[i] = (float)std::sqrt(err*err / n);
		totalErr += err*err;
		totalPoints += n;
	}

	return std::sqrt(totalErr / totalPoints);
}
//! [compute_errors]
//! [board_corners]
static void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners,
	Settings::Pattern patternType /*= Settings::CHESSBOARD*/)
{
	corners.clear();

	switch (patternType)
	{
	case Settings::CHESSBOARD:
	case Settings::CIRCLES_GRID:
		for (int i = 0; i < boardSize.height; ++i)
			for (int j = 0; j < boardSize.width; ++j)
				corners.push_back(Point3f(j*squareSize, i*squareSize, 0));
		break;

	case Settings::ASYMMETRIC_CIRCLES_GRID:
		for (int i = 0; i < boardSize.height; i++)
			for (int j = 0; j < boardSize.width; j++)
				corners.push_back(Point3f((2 * j + i % 2)*squareSize, i*squareSize, 0));
		break;
	default:
		break;
	}
}
//! [board_corners]
static bool runCalibration(Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
	vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
	vector<float>& reprojErrs, double& totalAvgErr, vector<Point3f>& newObjPoints,
	float grid_width, bool release_object)
{
	//! [fixed_aspect]
	cameraMatrix = Mat::eye(3, 3, CV_64F);
	if (!s.useFisheye && s.flag & CALIB_FIX_ASPECT_RATIO)
		cameraMatrix.at<double>(0, 0) = s.aspectRatio;
	//! [fixed_aspect]
	if (s.useFisheye) {
		distCoeffs = Mat::zeros(4, 1, CV_64F);
	}
	else {
		distCoeffs = Mat::zeros(8, 1, CV_64F);
	}

	vector<vector<Point3f> > objectPoints(1);
	calcBoardCornerPositions(s.boardSize, s.squareSize, objectPoints[0], s.calibrationPattern);
	objectPoints[0][s.boardSize.width - 1].x = objectPoints[0][0].x + grid_width;
	newObjPoints = objectPoints[0];

	objectPoints.resize(imagePoints.size(), objectPoints[0]);

	//Find intrinsic and extrinsic camera parameters
	double rms;

	if (s.useFisheye) {
		Mat _rvecs, _tvecs;
		rms = fisheye::calibrate(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, _rvecs,
			_tvecs, s.flag);

		rvecs.reserve(_rvecs.rows);
		tvecs.reserve(_tvecs.rows);
		for (int i = 0; i < int(objectPoints.size()); i++) {
			rvecs.push_back(_rvecs.row(i));
			tvecs.push_back(_tvecs.row(i));
		}
	}
	else {
		int iFixedPoint = -1;
		if (release_object)
			iFixedPoint = s.boardSize.width - 1;
		rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
			distCoeffs, rvecs, tvecs, s.flag | CALIB_FIX_K4 | CALIB_FIX_K5);
	}

	if (release_object) {
		cout << "New board corners: " << endl;
		cout << newObjPoints[0] << endl;
		cout << newObjPoints[s.boardSize.width - 1] << endl;
		cout << newObjPoints[s.boardSize.width * (s.boardSize.height - 1)] << endl;
		cout << newObjPoints.back() << endl;
	}

	cout << "Re-projection error reported by calibrateCamera: " << rms << endl;

	bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

	objectPoints.clear();
	objectPoints.resize(imagePoints.size(), newObjPoints);
	totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints, rvecs, tvecs, cameraMatrix,
		distCoeffs, reprojErrs, s.useFisheye);

	return ok;
}

// Print camera parameters to the output file
static void saveCameraParams(Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
	double totalAvgErr, const vector<Point3f>& newObjPoints)
{
	FileStorage fs(s.outputFileName, FileStorage::WRITE);

	time_t tm;
	time(&tm);
	struct tm *t2 = localtime(&tm);
	char buf[1024];
	strftime(buf, sizeof(buf), "%c", t2);

	fs << "calibration_time" << buf;

	if (!rvecs.empty() || !reprojErrs.empty())
		fs << "nr_of_frames" << (int)std::max(rvecs.size(), reprojErrs.size());
	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;
	fs << "board_width" << s.boardSize.width;
	fs << "board_height" << s.boardSize.height;
	fs << "square_size" << s.squareSize;

	if (!s.useFisheye && s.flag & CALIB_FIX_ASPECT_RATIO)
		fs << "fix_aspect_ratio" << s.aspectRatio;

	if (s.flag)
	{
		std::stringstream flagsStringStream;
		if (s.useFisheye)
		{
			flagsStringStream << "flags:"
				<< (s.flag & fisheye::CALIB_FIX_SKEW ? " +fix_skew" : "")
				<< (s.flag & fisheye::CALIB_FIX_K1 ? " +fix_k1" : "")
				<< (s.flag & fisheye::CALIB_FIX_K2 ? " +fix_k2" : "")
				<< (s.flag & fisheye::CALIB_FIX_K3 ? " +fix_k3" : "")
				<< (s.flag & fisheye::CALIB_FIX_K4 ? " +fix_k4" : "")
				<< (s.flag & fisheye::CALIB_RECOMPUTE_EXTRINSIC ? " +recompute_extrinsic" : "");
		}
		else
		{
			flagsStringStream << "flags:"
				<< (s.flag & CALIB_USE_INTRINSIC_GUESS ? " +use_intrinsic_guess" : "")
				<< (s.flag & CALIB_FIX_ASPECT_RATIO ? " +fix_aspectRatio" : "")
				<< (s.flag & CALIB_FIX_PRINCIPAL_POINT ? " +fix_principal_point" : "")
				<< (s.flag & CALIB_ZERO_TANGENT_DIST ? " +zero_tangent_dist" : "")
				<< (s.flag & CALIB_FIX_K1 ? " +fix_k1" : "")
				<< (s.flag & CALIB_FIX_K2 ? " +fix_k2" : "")
				<< (s.flag & CALIB_FIX_K3 ? " +fix_k3" : "")
				<< (s.flag & CALIB_FIX_K4 ? " +fix_k4" : "")
				<< (s.flag & CALIB_FIX_K5 ? " +fix_k5" : "");
		}
		fs.writeComment(flagsStringStream.str());
	}

	fs << "flags" << s.flag;

	fs << "fisheye_model" << s.useFisheye;

	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;

	fs << "avg_reprojection_error" << totalAvgErr;
	if (s.writeExtrinsics && !reprojErrs.empty())
		fs << "per_view_reprojection_errors" << Mat(reprojErrs);

	if (s.writeExtrinsics && !rvecs.empty() && !tvecs.empty())
	{
		CV_Assert(rvecs[0].type() == tvecs[0].type());
		Mat bigmat((int)rvecs.size(), 6, CV_MAKETYPE(rvecs[0].type(), 1));
		bool needReshapeR = rvecs[0].depth() != 1 ? true : false;
		bool needReshapeT = tvecs[0].depth() != 1 ? true : false;

		for (size_t i = 0; i < rvecs.size(); i++)
		{
			Mat r = bigmat(Range(int(i), int(i + 1)), Range(0, 3));
			Mat t = bigmat(Range(int(i), int(i + 1)), Range(3, 6));

			if (needReshapeR)
				rvecs[i].reshape(1, 1).copyTo(r);
			else
			{
				//*.t() is MatExpr (not Mat) so we can use assignment operator
				CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
				r = rvecs[i].t();
			}

			if (needReshapeT)
				tvecs[i].reshape(1, 1).copyTo(t);
			else
			{
				CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
				t = tvecs[i].t();
			}
		}
		fs.writeComment("a set of 6-tuples (rotation vector + translation vector) for each view");
		fs << "extrinsic_parameters" << bigmat;
	}

	if (s.writePoints && !imagePoints.empty())
	{
		Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
		for (size_t i = 0; i < imagePoints.size(); i++)
		{
			Mat r = imagePtMat.row(int(i)).reshape(2, imagePtMat.cols);
			Mat imgpti(imagePoints[i]);
			imgpti.copyTo(r);
		}
		fs << "image_points" << imagePtMat;
	}

	if (s.writeGrid && !newObjPoints.empty())
	{
		fs << "grid_points" << newObjPoints;
	}
}

//! [run_and_save]
bool runCalibrationAndSave(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
	vector<vector<Point2f> > imagePoints, float grid_width, bool release_object)
{
	vector<Mat> rvecs, tvecs;
	vector<float> reprojErrs;
	double totalAvgErr = 0;
	vector<Point3f> newObjPoints;

	bool ok = runCalibration(s, imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs, reprojErrs,
		totalAvgErr, newObjPoints, grid_width, release_object);
	cout << (ok ? "Calibration succeeded" : "Calibration failed")
		<< ". avg re projection error = " << totalAvgErr << endl;

	if (ok)
		saveCameraParams(s, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, reprojErrs, imagePoints,
			totalAvgErr, newObjPoints);
	return ok;
}
//! [run_and_save]
