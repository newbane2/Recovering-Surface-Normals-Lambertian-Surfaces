/*
    Allan Spektor S1
	iterates through image and sets pixel to 0 or 255 based on threshold (makes it a binary image)
	then calculates the centroid (center x,y) and the radius and outputs it into a file
*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "image.h"

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv) 
{

	if (argc != 4) {
		printf("Usage: %s {input original image} {input threshold value} {output parameters file}\n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
	const int threshold = stoi(argv[2]);
	const string output_file(argv[3]);

	Image image;

	if (!ReadImage(input_file, &image)) {
		cout << "Can't open file " << input_file << endl;
		return 0;
	}

	//iterates through image and sets pixel to 0 or 255 based on threshold
	int rMin = image.num_rows(), rMax = 0, cMin = image.num_columns(), cMax = 0, centerRow = 0, centerColumn = 0, count = 0;
	for (int i = 0; i < image.num_rows(); i++) {
		for (int j = 0; j < image.num_columns(); j++) {
			if (image.GetPixel(i, j) < threshold) {
				image.SetPixel(i, j, 0);
			} else {
				rMin = min(rMin, i); rMax = max(rMax, i);
				cMin = min(cMin, j); cMax = max(cMax, j);
				centerRow += i; centerColumn += j;
				image.SetPixel(i, j, 255);
				count++;
			}
		}
	}

	int centerX = round(centerRow / count);
	int centerY = round(centerColumn / count);

	int radius = round((rMax - rMin + cMax - cMin) / 4.0);
	ofstream outFile(output_file);
	if(outFile.fail()) {
		cout << "Failed opening output file " << output_file << endl;
		abort();
	} else {
		outFile << centerX << " " << centerY << " " << radius;
	}
	outFile.close();

	cout << "done" << endl;
}