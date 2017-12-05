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


class LineSegmentation {
public:
	/**
	 * Segment the given text image into lines.
	 */
	static void segment(const cv::Mat& img, vector<cv::Mat>& lines);

private:
	/**
	 * Private constructor (i.e. static class).
	 */
	LineSegmentation() {}

	/**
	 * Divide the given line into words according to the given space threshold.
	 */
	static void divideImage(const cv::Mat& img, vector<cv::Mat>& lines, int threshold);
};