#ifndef __APP__
#define __APP__

#include <Windows.h>
#include <comutil.h>
#include <iostream>
#include <cstdio>
#include <ctime>

#include <wtypes.h>
#include <comdef.h> 
#include <string>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include "atlbase.h"
#include "atlwin.h"
#include <Kinect.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <vector>

#include <wrl/client.h>
using namespace Microsoft::WRL;
using namespace cv;
using namespace std;

class Capture {
private :
	// Sensor
	ComPtr<IKinectSensor> kinect;

	// Reader
	ComPtr<IColorFrameReader> colorFrameReader;

	// Color Buffer
	std::vector<BYTE> colorBuffer;
	int colorWidth;
	int colorHeight;
	unsigned int colorBytesPerPixel;

	cv::Mat colorMat;

	// OpenCV Chessboard parameters. Change these values if you use a different 
	int numOfCornersHorizontal; // It is 9 by default
	int numOfCornersVertical; // It is 6 by default
	int numOfBoardImages;
	Size boardSize;
	float squareSize; // Actual length of one square edge in the chessboard pattern. In this example it is 2.6cm
	int numOfCalibrationLoop; // It is 30 by default;
	vector< vector< Point3f > > cornerPointsInrealWorld;
	vector< vector< Point2f > > cornerPointsOnImage;
	std::clock_t startClockTick;
	double timeDelayBeforeCalibration; // Default : 2 secs
	int numOfSuccessfulCornerDetections;
	int maxNumOfSuccessfulCornerDetections; // Default : 10 images

public:
	// Constructor
	Capture();

	// Destructor
	~Capture();
	void run();

private :
	void initialize();
	void finalize();

	inline void initializeSensor();
	inline void initializeColorImage();
	void update();
	inline void updateColor();
	inline void captureSampleImages(Mat image);
	inline void calibrate();
	void draw();
	inline void drawColor();
	void show();
	inline void showColor();

	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	inline void doMouseCallback(int event, int x, int y, int flags);

	void setupCalibration();
};

#endif // __APP__