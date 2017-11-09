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
		throw exception(errorMessage.c_str());
	}

	// Binary image
	cv::Mat binaryMat(grayMat.size(), grayMat.type());

	// Apply thresholding
	cv::threshold(grayMat, binaryMat, THRESHOLD, 255, cv::THRESH_BINARY);
	
	return binaryMat;
}

/**
 * Compare the given two images and return true if they match,
 * false otherwise
 */
inline bool compareImages(const cv::Mat& img1, const cv::Mat& img2) {
	// treat two empty mat as identical
	if (img1.empty() && img2.empty())
		return true;

	// check the dimensions of the two matrices
	if (img1.cols != img2.cols || img1.rows != img2.rows || img1.dims != img2.dims)
		return false;

	// Compare every element in the two matrices
	for (int i = 0; i < img1.rows; ++i)
		for (int j = 0; j < img1.cols; ++j)
			if (img1.at<bool>(i, j) != img2.at<bool>(i, j))
				return false;

	return true;
}