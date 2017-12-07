#pragma once

// STL libraries
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

// OpenCV libraries
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

// Custom libraries
#include "Constants.h"

using namespace std;
using namespace cv;


class Utilities {
public:
	/**
	 * Load image from the given path and 
	 * return it in a matrix of pixels form.
	 */
	static cv::Mat loadImage(const string& path);

	/**
	 * Resize the given image if its size is above the maximum limit
	 * while keeping the original aspect ratio.
	 */
	static void resizeImage(Mat& img, int maxWidth, int maxHeight);

	/**
	 * Perform gamma correction function to the given image.
	 */
	static void gammaCorrection(Mat& src, double gamma);

	/**
	 * Preprocess the given image.
	 */
	static cv::Mat preprocess(cv::Mat& img);

	/**
	 * Postprocess the given code string.
	 */
	static string postprocess(const string& str);

	/**
	 * Make a new directory at the given path.
	 */
	static void makeDir(const string& dir);

	/**
	 * Remove the given directory recursively.
	 */
	static void removeDir(const string& dir);
};