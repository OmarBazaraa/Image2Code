#include "Utilities.h"

cv::Mat Utilities::loadImage(const string& path) {
	// Load grayscale image from file
	Mat img = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	// Check for invalid input
	if (img.empty() || !img.data) {
		cout << path;
		string errorMessage = "Could not load the image at: " + path;
		throw exception();
	}

	return img;
}

cv::Mat Utilities::preprocess(const cv::Mat& img) {
	// Noise reduction (Currently a basic filter).
	cv::Mat smoothedImg(img.size(), img.type());
	GaussianBlur(img, smoothedImg, Size(3, 3), 2, 2);
	
	imwrite("Data\\blured.png", smoothedImg);
	
	// Apply basic thresholding
	cv::Mat binaryImg(img.size(), img.type());
	cv::threshold(smoothedImg, binaryImg, 100, 255, cv::THRESH_BINARY);
	
	imwrite("Data\\thresholded.png", binaryImg);
	
	// Connect characters
	cv::Mat dilatedImg, erodedImg;
	cv::Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	cv::dilate(binaryImg, dilatedImg, element);
	cv::erode(dilatedImg, erodedImg, element);
	
	imwrite("Data\\eroded.png", binaryImg);
	imwrite("Data\\dilated.png", binaryImg);
	
	return erodedImg;

	//// Apply adaptive Gaussian thresholding
	//cv::Mat binaryImg(img.size(), img.type());
	//cv::adaptiveThreshold(img, binaryImg, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 25, 0.5);
	//
	//imwrite("Data\\thresholded.png", binaryImg);
	//
	//// Apply median filter to remove impulsive noise
	//cv::Mat filterdImg;
	//cv::medianBlur(binaryImg, filterdImg, 3);
	//
	//imwrite("Data\\filterd.png", filterdImg);
	//
	//// Connect characters
	//cv::Mat dilatedImg, erodedImg;
	//cv::Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	//cv::erode(filterdImg, erodedImg, element);
	//cv::dilate(erodedImg, dilatedImg, element);
	//
	//imwrite("Data\\eroded.png", binaryImg);
	//imwrite("Data\\dilated.png", binaryImg);
	//
	//return dilatedImg;
}

string Utilities::postprocess(const string& str) {
	return str;
}

void Utilities::makeDir(const string& dir) {
	string cmd = "mkdir " + dir;
	system(cmd.c_str());
}

void Utilities::removeDir(const string& dir) {
#ifdef _WIN32
	string cmd = "rmdir " + dir + " /S/Q";
#else
	string cmd = "rm -r " + dir;
#endif

	system(cmd.c_str());
}