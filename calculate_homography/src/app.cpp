#include "app.h"
#include "util.h"
#include <thread>
#include <chrono>

#include <numeric>

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>


using namespace std;

// Constructor
Capture::Capture()
{
	// Initialize
	initialize();
}

// Destructor
Capture::~Capture()
{
	// Finalize
	finalize();
}

void Capture::initialize() {
	
	cvNamedWindow("Color", CV_WINDOW_NORMAL);
	cvNamedWindow("Detected", CV_WINDOW_NORMAL);

	initializeSensor();
	initializeColorImage();
	cv::setMouseCallback("Color", mouseCallback, this);

	setupCalibration();
	
}

void Capture::finalize() {

}

void Capture::run()
{
	while (true) {
		update();
		draw();
		show();
		if(waitKey(1) == 113) break;

		if (waitKey(1) == 99) {
			// Calibrate next image sample
			captureSampleImages(colorMat);
		}

		if (numOfSuccessfulCornerDetections >= maxNumOfSuccessfulCornerDetections) break;
	}

	if (cornerPointsInrealWorld.size() == cornerPointsOnImage.size() && cornerPointsOnImage.size() > 0) {
		calibrate();
	}
}

// Initialize Sensor
inline void Capture::initializeSensor()
{
	// Open Sensor
	ERROR_CHECK(GetDefaultKinectSensor(&kinect));

	ERROR_CHECK(kinect->Open());

	// Check Open
	BOOLEAN isOpen = FALSE;
	ERROR_CHECK(kinect->get_IsOpen(&isOpen));
	if (!isOpen) {
		throw std::runtime_error("failed IKinectSensor::get_IsOpen( &isOpen )");
	}
}

// Initialize color image
inline void Capture::initializeColorImage()
{
	// Open Color Reader
	ComPtr<IColorFrameSource> colorFrameSource;
	ERROR_CHECK(kinect->get_ColorFrameSource(&colorFrameSource));
	ERROR_CHECK(colorFrameSource->OpenReader(&colorFrameReader));

	// Retrieve Color Description
	ComPtr<IFrameDescription> colorFrameDescription;
	ERROR_CHECK(colorFrameSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Bgra, &colorFrameDescription));
	ERROR_CHECK(colorFrameDescription->get_Width(&colorWidth)); // 1920
	ERROR_CHECK(colorFrameDescription->get_Height(&colorHeight)); // 1080
	ERROR_CHECK(colorFrameDescription->get_BytesPerPixel(&colorBytesPerPixel)); // 4

	// Allocation of Color Buffer
	colorBuffer.resize(colorWidth * colorHeight * colorBytesPerPixel);
}

// Update Data
void Capture::update()
{
	// Update Color
	updateColor();
}

// Update Color
inline void Capture::updateColor()
{
	// Retrieve Color Frame
	ComPtr<IColorFrame> colorFrame;
	const HRESULT ret = colorFrameReader->AcquireLatestFrame(&colorFrame);
	if (FAILED(ret)) {
		return;
	}

	// Convert Format ( YUY2 -> BGRA )
	ERROR_CHECK(colorFrame->CopyConvertedFrameDataToArray(static_cast<UINT>(colorBuffer.size()), &colorBuffer[0], ColorImageFormat::ColorImageFormat_Bgra));
}

inline void Capture::captureSampleImages(Mat image) {

	if ((clock() - startClockTick) / (double)CLOCKS_PER_SEC < timeDelayBeforeCalibration) {
		return;
	}
	printf("Capturing sample images\n");
	// Convert to Grayscale
	
	vector<Point2f> pointBuf;
	int chessBoardFlags = CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE;
	Mat gray;
	cv::cvtColor(image, gray, CV_BGR2GRAY);
	imshow("Detected", gray);
	vector< Point2f > corners;
	bool found = false;
	
	found = findChessboardCorners(image, boardSize, corners, chessBoardFlags);
	
	if (found)
	{
		cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1),
			TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(gray, boardSize, corners, found);
		imshow("Detected", gray);
	}
	else {
		return;
	}
	
	vector< Point3f > obj;
	for (int i = 0; i < numOfCornersVertical; i++)
		for (int j = 0; j < numOfCornersHorizontal; j++)
			obj.push_back(Point3f((float)j * squareSize, (float)i * squareSize, 0));

	if (found) {
		cornerPointsOnImage.push_back(corners);
		cornerPointsInrealWorld.push_back(obj);
		numOfSuccessfulCornerDetections++;
	}
	
}

// Draw Data
void Capture::draw()
{

	// Draw Color
	drawColor();

}

// Draw Color
inline void Capture::drawColor()
{
	// Convert Coordinate Buffer to cv::Mat
	//Mapping color and depth frame taken from https://gist.github.com/UnaNancyOwen/7e2c685752e16f8e42cc#file-main-cpp-L232
	colorMat = Mat(colorHeight, colorWidth, CV_8UC4, &colorBuffer[0]).clone();
	cv::flip(colorMat, colorMat, 1);
}

// Show Data
void Capture::show()
{
	// Show Color
	showColor();

}

// Show Color
inline void Capture::showColor()
{
	if (colorMat.empty()) {
		return;
	}
	
	cv::imshow("Color", colorMat);
}



void Capture::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		Capture* self = static_cast<Capture*>(userdata);
		self->doMouseCallback(event, x, y, flags);
	}
}

void Capture::doMouseCallback(int event, int x, int y, int flags) {
	if (flags == (cv::EVENT_FLAG_LBUTTON))
	{
		std::cout << "Left mouse clicked" << std::endl;
	}

	if (flags == (cv::EVENT_FLAG_LBUTTON + cv::EVENT_FLAG_SHIFTKEY))
	{
		
		std::cout << "Shift+Left click" << std::endl;
	}

}

void Capture::setupCalibration() {
	numOfCornersHorizontal = 9;
	numOfCornersVertical = 6;
	numOfCalibrationLoop = 30;
	squareSize = 2.6;
	maxNumOfSuccessfulCornerDetections = 10;
	numOfSuccessfulCornerDetections = 0;
	boardSize = Size(numOfCornersHorizontal, numOfCornersVertical);
	int board_n = numOfCornersHorizontal * numOfCornersVertical;
	timeDelayBeforeCalibration = 2;
	startClockTick = clock();
}

void Capture::calibrate() {
	Mat K = Mat(3, 3, CV_32FC1);;
	Mat D;
	vector< Mat > rvecs, tvecs;
	int flag = 0;
	K.ptr<float>(0)[0] = 1; 
	K.ptr<float>(1)[1] = 1; 
	flag |= CV_CALIB_FIX_K4;
	flag |= CV_CALIB_FIX_K5;
	printf("Calibrating ...");
	calibrateCamera(cornerPointsInrealWorld, cornerPointsOnImage, colorMat.size(), K, D, rvecs, tvecs, flag);
	
	FileStorage fs("camera_parameters.yml", FileStorage::WRITE);
	fs << "K" << K;
	fs << "D" << D;
	fs << "board_width" << numOfCornersHorizontal;
	fs << "board_height" << numOfCornersVertical;
	fs << "square_size" << squareSize;
	printf("Done Calibration\n");
	
}