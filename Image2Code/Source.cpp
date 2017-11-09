// STL libraries
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

// Custom libraries
#include "Utilities\Directory.h"
#include "Utilities\Utility.h"
#include "Segmentation\CharSegmentor.h"
using namespace cv;

// Pathes
const string PATH_INPUT = "Data\\Input\\";
const string PATH_OUTPUT = "Data\\Output\\";

// Output the given character matrix vector to the given path
void saveChars(const vector<cv::Mat>& chars, const string& path, const string& extension) {
	// Create new folder
	// Note: Windows command used
	string cmd = "mkdir " + path;
	system(cmd.c_str());

	// Save every segmented char
	for (int i = 0; i < chars.size(); ++i) {
		string dir = path + to_string(i) + "." + extension;
		imwrite(dir, chars[i]);
	}
}

// Main program driver function
int main() {
	int startTime = clock();
	vector<pair<string, string>> files;

	// Delete previous output directory
	// Note: Windows command used
	string cmd = "rmdir " + PATH_OUTPUT + " /S/Q";
	system(cmd.c_str());

	try {
		// Read files info
		getFilesInDirectory(PATH_INPUT, files);

		for (int i = 0; i < files.size(); ++i) {
			// If file is not of type .jpg or .png then skip it
			if (files[i].second != "jpg" && files[i].second != "png") {
				continue;
			}

			// Get file pathes
			string src = PATH_INPUT + files[i].first + "." + files[i].second;
			string dst = PATH_OUTPUT + files[i].first + "\\";

			// Segmentation variables
			cv::Mat word;
			vector<cv::Mat> chars;

			// Loading image
			cout << "Loading " << src << "..." << endl;
			word = loadBinaryImage(src);

			// Compressing
			cout << "Segmenting characters..." << endl;
			CharSegmentor::segment(word, chars);
			cout << "Character segmentation done with " << chars.size() << " chars" << endl;

			// Saving segmented chars
			cout << "Saving segmented chars..." << endl;
			saveChars(chars, dst, files[i].second);
		}
	}
	catch (const exception& ex) {
		cout << "ERROR::" << ex.what() << endl;
	}

	// Output process time
	int stopTime = clock();
	cout << "Time: " << (stopTime - startTime) / double(CLOCKS_PER_SEC) << "sec" << endl;
	return 0;
}