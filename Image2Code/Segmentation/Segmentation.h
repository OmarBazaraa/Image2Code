#pragma once

// STL libraries
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
	static cv::Mat rgbImg;
	static cv::Mat binaryImg;
	static cv::Mat segmentedImg;
	static cv::Mat redImg;
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
	static void processChar(cv::Mat& charImg, const string& imgName);

	/**
	 * Draw rectangle around the word and its characters.
	 */
	static void highlightWord(cv::Mat& wordImg);
	
	/**
	 * Preprocess the given image.
	 */
	static void preprocess();

	/**
	 * Postprocess the given code string.
	 */
	static void postprocess();
};