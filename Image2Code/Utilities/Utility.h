#pragma once
// STL libraries
#include <string>
using namespace std;

// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

const int THRESHOLD = 200;

/**
 * Loads binary image from the given path into a matrix of pixels
 */
inline cv::Mat loadBinaryImage(const string& path) {
	// Load gray scale image from file
	cv::Mat grayMat = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	// Check for invalid input
	if (grayMat.empty() || !grayMat.data) {
		string errorMessage = "Could not load the image at: " + path;
		throw exception();
	}

	// Binary image
	cv::Mat binaryMat(grayMat.size(), grayMat.type());

	// Apply thresholding
	cv::threshold(grayMat, binaryMat, THRESHOLD, 255, cv::THRESH_BINARY);
	
	return binaryMat;
}