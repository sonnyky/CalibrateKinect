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

	initializeSensor();
	initializeColorImage();
	cv::setMouseCallback("Color", mouseCallback, this);
	
}

void Capture::finalize() {

}

void Capture::run()
{
	while (true) {
		update();
		draw();
		show();
		if (waitKey(1) == 99) {
			vector<Point2f> pts_src, pts_dest;
			pts_src.push_back(topLeft); pts_src.push_back(topRight); pts_src.push_back(bottomLeft); pts_src.push_back(bottomRight);
			pts_dest.push_back(topLeftImage); pts_dest.push_back(topRightImage); pts_dest.push_back(bottomLeftImage); pts_dest.push_back(bottomRightImage);
			calcHomographyMatrix(pts_src, pts_dest);
		}else if(waitKey(1) == 113) break;

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
	if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_RBUTTONDOWN)
	{
		Capture* self = static_cast<Capture*>(userdata);
		self->doMouseCallback(event, x, y, flags);
	}
}

void Capture::doMouseCallback(int event, int x, int y, int flags) {
	if (flags == (cv::EVENT_FLAG_LBUTTON))
	{
		std::cout << "Left mouse clicked, registering point as top left point" << std::endl;
		topLeftImage.x = x;
		topLeftImage.y = y;
	}

	if (flags == (cv::EVENT_FLAG_LBUTTON + cv::EVENT_FLAG_SHIFTKEY))
	{
		std::cout << "Shift+Left clicked, registering point as top right point" << std::endl;
		topRightImage.x = x;
		topRightImage.y = y;
	}

	if (flags == (cv::EVENT_FLAG_RBUTTON))
	{
		std::cout << "Right mouse clicked, registering point as bottom left point" << std::endl;
		bottomLeftImage.x = x;
		bottomLeftImage.y = y;
	}

	if (flags == (cv::EVENT_FLAG_RBUTTON + cv::EVENT_FLAG_SHIFTKEY))
	{

		std::cout << "Shift+Right clicked, registering point as bottom right point" << std::endl;
		bottomRightImage.x = x;
		bottomRightImage.y = y;
	}

}

void Capture::setRangePoints(int topLeftX, int topLeftY, int topRightX, int topRightY, int bottomLeftX, int bottomLeftY, int bottomRightX, int bottomRightY) {
	topLeft.x = topLeftX;
	topLeft.y = topLeftY;
	topRight.x = topRightX;
	topRight.y = topRightY;
	bottomLeft.x = bottomLeftX;
	bottomLeft.y = bottomLeftY;
	bottomRight.x = bottomRightX;
	bottomRight.y = bottomRightY;
	printf("Top left x : %f\n", topLeft.x);
	printf("Top left y : %f\n", topLeft.y);
	printf("Top right x : %f\n", topRight.x);
	printf("Top right y : %f\n", topRight.y);
	printf("Bottom left x : %f\n", bottomLeft.x);
	printf("Bottom left y : %f\n", bottomLeft.y);
	printf("Bottom right x : %f\n", bottomRight.x);
	printf("Bottom right y : %f\n", bottomRight.y);
}

void Capture::calcHomographyMatrix(vector<Point2f> pts_src, vector<Point2f> pts_dest) {
	printf("Calculating homography...\n Please redo clicking if the results are not as expected.\n Matrix will be output as text file.");
	Mat h = findHomography(pts_src, pts_dest);
	FileStorage file("homography.xml", 1, "UTF-8");
	file <<"Homography" << h;
}