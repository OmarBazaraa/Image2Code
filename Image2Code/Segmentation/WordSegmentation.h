#pragma once

// STL libraries
#include <vector>

// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Custom libraries
#include "../Utilities/Constants.h"

using namespace std;
using namespace cv;


class WordSegmentation {
private:
	static int rows, cols, L, R;
	static cv::Mat line;
	static vector<int> pixelsCount;
	static vector<pair<int, int>> whiteSpaces;

public:
	/**
	 * Segment the given line image into words.
	 */
	static void segment(cv::Mat& img, vector<cv::Mat>& words);

private:
	/**
	 * Private constructor (i.e. static class).
	 */
	WordSegmentation() {}

	/**
	 * Calculate space threshold between words.
	 */
	static int calcSpaceThreshold();

	/**
	 * Detect the white spaces in the given line image.
	 */
	static void detectWhiteSpaces(int threshold);

	/**
	 * Divide the given line into words according to the white spaces.
	 */
	static void divideLine(vector<cv::Mat>& words);

	/**
	 * Initialize word segmentation variables.
	 */
	static void init(cv::Mat& img);
};