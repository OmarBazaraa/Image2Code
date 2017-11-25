#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/**
 * Returns a list of files (name, extension) in the given directory
 */
inline void getFilesInDirectory(const string& directory, vector<pair<string, string>>& files) {
	// Make sure to change the following command to the corresponding
	// one on your operating system when using Linux or MAC
	string txtPath = "../Data/Input/dirs.txt";

	string s = "ls " + directory;
	system(s.c_str());

	ifstream fin(txtPath);

	while (getline(fin, s)) {
		int idx = (int)s.size() - 1;
		while (idx >= 0 && s[idx] != '.') --idx;
		files.push_back({ s.substr(0, idx), s.substr(idx + 1) });
	}

//	remove(txtPath.c_str());
}