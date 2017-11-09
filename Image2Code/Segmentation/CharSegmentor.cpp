#pragma once
#include "CharSegmentor.h"

//
// Static members declaration
//
int CharSegmentor::n;
int CharSegmentor::m;
int CharSegmentor::id;
cv::Mat CharSegmentor::word;
cv::Mat CharSegmentor::visited;
vector<Region> CharSegmentor::regions;
map<int, int> CharSegmentor::regionsID;
Region CharSegmentor::region;
//========================================================================

/**
 * Segment the given word image into characters and add them
 * to the given chars vector.
 */
void CharSegmentor::segment(cv::Mat& img, vector<cv::Mat>& chars) {
	// Initialization
	init(img);

	// Scan word matrix
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			// Continue if previously visited or background pixel
			if (visited.at<uchar>(i, j) || word.at<uchar>(i, j) == BACKCOLOR) {
				continue;
			}

			// Get region boundries	
			region = Region();
			region.id = ++id;
			regionsID[id] = id;
			dfs(i, j);

			// Store character region
			regions.push_back(region);
		}
	}

	mergeRegions();
	extractChars(chars);
}

/**
 * Extract regions of the same id into a matrix
 * and add it to the chars vector.
 */
void CharSegmentor::extractChars(vector<cv::Mat>& chars) {
	for (auto& r : regions) {
		int w = r.R - r.L + 1;
		int h = r.D - r.U + 1;
		cv::Mat m(h, w, CV_8U, cv::Scalar(BACKCOLOR));

		for (int i = r.U; i <= r.D; ++i) {
			for (int j = r.L; j <= r.R; ++j) {
				id = visited.at<uchar>(i, j);

				if (regionsID[id] == r.id) {
					m.at<uchar>(i - r.U, j - r.L) = FORECOLOR;
				}
			}
		}

		chars.push_back(m);
	}
}

/**
 * Merge overlapping regions into one region.
 */
void CharSegmentor::mergeRegions() {
	vector<Region> tmp;
	sort(regions.rbegin(), regions.rend());

	for (int i = 0; i < regions.size(); ++i) {
		Region& p = regions[i];

		for (i++; i < regions.size(); ++i) {
			Region& q = regions[i];
			
			int commonWidth = q.R - max(p.L, q.L) + 1;

			if (commonWidth * 100 < (p.R - p.L + 1) * MERGE_THRESHOLD) {
				i--;
				break;
			}

			regionsID[q.id] = p.id;
			p.merge(q);
		}

		tmp.push_back(p);
	}

	reverse(tmp.begin(), tmp.end());
	regions.swap(tmp);
}

/**
 * Start depth first search from the given point to get the boundaries
 * of the connected region and mark it with a unique id.
 */
void CharSegmentor::dfs(int row, int col) {
	// Update boundries
	region.U = min(region.U, row);
	region.L = min(region.L, col);
	region.D = max(region.D, row);
	region.R = max(region.R, col);

	// Set current pixel as visisted
	visited.at<uchar>(row, col) = id;

	// Visit neighbours
	for (int i = 0; i < 8; ++i) {
		int toR = row + dirR[i];
		int toC = col + dirC[i];

		if (valid(toR, toC) && !visited.at<uchar>(toR, toC)) {
			dfs(toR, toC);
		}
	}
}

/**
 * Check if the given pixel is inside the image
 * and that its color is foreground color.
 */
bool CharSegmentor::valid(int row, int col) {
	return (
		row >= 0 && row < n &&
		col >= 0 && col < m &&
		word.at<uchar>(row, col) == FORECOLOR
	);
}

/**
 * Initialize character segmentation variables.
 */
void CharSegmentor::init(cv::Mat& img) {
	n = img.rows;
	m = img.cols;
	id = 0;
	word = img;
	visited = cv::Mat::zeros(n, m, CV_8U);
	regions.clear();
	regionsID.clear();
}