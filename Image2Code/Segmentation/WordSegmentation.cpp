#include "WordSegmentation.h"

#include <iostream>

//
// Static members declaration
//
int WordSegmentation::rows;
int WordSegmentation::cols;
int WordSegmentation::L;
int WordSegmentation::R;
cv::Mat WordSegmentation::line;
vector<int> WordSegmentation::pixelsCount;
vector<pair<int, int>> WordSegmentation::whiteSpaces;
//========================================================================


void WordSegmentation::segment(cv::Mat& img, vector<cv::Mat>& words) {
	init(img);
	int spaceThreshold = calcSpaceThreshold();
	detectWhiteSpaces(spaceThreshold);
	divideLine(words);
}

int WordSegmentation::calcSpaceThreshold() {
	int sum, num, width;

	//
	// Calculate average characters width
	//
	sum = num = width = 0;
	for (int i = L; i <= R; ++i) {
		// Character
		if (pixelsCount[i] > 0) {
			width++;
		}
		// Space
		else if (width > 0) {
			sum += width;
			num++;
			width = 0;
		}
	}
	
	// Take the last character into consideration 
	if (width > 0) {
		sum += width;
		num++;
	}

	num = max(1, num);
	int avgCharWidth = sum / num;
	// -------------------------------------------------------------------------

	//
	// Calculate average white spaces width that are greater than @avgCharWidth
	//
	sum = num = width = 0;
	for (int i = L; i <= R; ++i) {
		// Space
		if (pixelsCount[i] == 0) {
			width++;
		}
		// Character
		else {
			if (width > avgCharWidth) {
				sum += width;
				num++;
			}

			if (width > 0) {
				std::cout << width << ' ';
			}
			
			width = 0;
		}
	}

	cout << endl;

	num = max(1, num);
	int avgSpaceWidth = (sum + num - 1) / num;
	// -------------------------------------------------------------------------

	// Calculate the threshold as the average of the two widthes
	int thresh = (avgCharWidth + avgSpaceWidth + 1) / 2;
	return thresh;
}

void WordSegmentation::detectWhiteSpaces(int threshold) {
	int prv = 0, cnt = 0;
	whiteSpaces.clear();

	// Add left padding
	whiteSpaces.push_back({ 0, L - 1 });

	// Detect white spaces
	for (int i = L; i <= R; ++i) {
		// Character
		if (pixelsCount[i] > 0) {
			if (cnt > threshold) {
				whiteSpaces.push_back({ prv, i - 1 });
			}
			cnt = 0;
		}
		// Space
		else if (cnt++ == 0) {
			prv = i;
		}
	}

	// Add right padding
	whiteSpaces.push_back({ R + 1, cols - 1 });
}

void WordSegmentation::divideLine(vector<cv::Mat>& words) {
	// Detect words between two successive white spaces
	for (int i = 1; i < whiteSpaces.size(); ++i) {
		int l = whiteSpaces[i - 1].second + 1;
		int r = whiteSpaces[i].first - 1;

		if (r <= l) {
			continue;
		}

		Mat word = line(cv::Rect(l, 0, r - l + 1, rows));
		words.push_back(word);
	}
}

void WordSegmentation::init(cv::Mat& img) {
	rows = img.rows;
	cols = img.cols;
	line = img;
	L = 0;
	R = cols - 1;
	pixelsCount.clear();
	pixelsCount.resize(cols, 0);

	// Calculate black pixels count in each column
	for (int i = 0; i < cols; ++i) {
		for (int j = 0; j < rows; ++j) {
			pixelsCount[i] += (img.at<uchar>(j, i) == FORECOLOR);
		}
	}

	// Detect line left and right paddings
	while (L <= R && pixelsCount[L] == 0) {
		L++;
	}
	while (R >= L && pixelsCount[R] == 0) {
		R--;
	}
}