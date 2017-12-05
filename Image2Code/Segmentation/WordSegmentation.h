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
public:
	/**
	 * Segment the given line image into words.
	 */
	static void segment(const cv::Mat& img, vector<cv::Mat>& words);

private:
	/**
	 * Private constructor (i.e. static class).
	 */
	WordSegmentation() {}

	/**
	 * Divide the given line into words according to the given space threshold.
	 */
	static void divideLine(const cv::Mat& img, vector<cv::Mat>& words, int threshold);
};