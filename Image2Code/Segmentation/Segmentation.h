#pragma once

// STL libraries
#include <iostream>
#include <vector>

// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Custom libraries
#include "LineSegmentation.h"
#include "WordSegmentation.h"
#include "CharSegmentation.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Utilities.h"

using namespace std;
using namespace cv;


class Segmentation {
public:
	static cv::Mat colorImg;
	static cv::Mat binaryImg;
	static cv::Mat redImg;
	static cv::Mat segmentedImg;
	static string code;

private:
	static int avgCharWidth;

public:
	/**
	 * Segment the given text image into lines.
	 */
	static void segment(cv::Mat& img);

private:
	/**
	 * Private constructor (i.e. static class).
	 */
	Segmentation() {}

	/**
	 * Preprocess lines, words and character of the given image.
	 */
	static void processLine(cv::Mat& lineImg, const string& imgName);
	static void processWord(cv::Mat& wordImg, const string& imgName);
	static void processChar(cv::Mat& charImg, const string& imgName, int offsetX, int offsetY, bool special);
	
	/**
	 * Preprocess the given image.
	 */
	static void preprocess();

	/**
	 * Rotate the image to correct the skewness
	 */
	static void skewCorrection();
};