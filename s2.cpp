/*
    Allan Spektor S2
	given the center point and the radius, searches in the circle in 3 separate images, 
    finds the brightest point, then uses that information to find the normalized vector of 
    the light source in all 3 images, and then outputs these values to an output file.
*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include "image.h"

using namespace std;
using namespace ComputerVisionProjects;

namespace ComputerVisionProjects {
    struct NormalVector {
        double x, y, z;
    };
    struct CircleInfo {
        int x, y, radius;
    };
}


void openImageFile(string path, Image & image);
NormalVector calculateNormalVector(Image & image, const CircleInfo & cInfo);
pair<pair<int,int>,int> getBrightestPoint(Image & image, const CircleInfo & cInfo);

int main(int argc, char **argv) 
{

	if (argc != 6) {
		printf("Usage: %s {input parameters file} {image 1} {image 2} {image 3} {output directions file}\n", argv[0]);
		return -1;
	}

	const string input_file(argv[1]); // x, y, radius
    const vector<string> imageFilePaths = {argv[2], argv[3], argv[4]};
	const string output_file(argv[5]);

    CircleInfo cInfo;
    vector<Image> images(3);
    ifstream iFile(input_file);

    //loads circle info
    if(!iFile.fail()) {
        iFile >> cInfo.x >> cInfo.y >> cInfo.radius;
        iFile.close();
    } else {
        cout << "Failed opening " << input_file << endl;
        return -1;
    }

    //loads the 3 pgm files into image
    for(int i = 0; i < 3; i++) {
        openImageFile(imageFilePaths[i], images[i]);
    }

    //calculates the 3 normal vectors
    vector<NormalVector> normalVectors = {calculateNormalVector(images[0], cInfo), calculateNormalVector(images[1], cInfo), calculateNormalVector(images[2], cInfo)};
    
    ofstream oFile(output_file.c_str());
    if(oFile.fail()) {
        cout << "Failed opening output file " << output_file << endl;
        return -1;
    }

    //outputs these vectors to an output.txt
    for(auto & i : normalVectors) {
        oFile << i.x << " " << i.y << " " << i.z << endl;
    }
    oFile.close();

	cout << "done" << endl;
}

//calculates the normalized vectors for each image
NormalVector calculateNormalVector(Image & image, const CircleInfo & cInfo) {
    pair<pair<int,int>,int> brightestPoint = getBrightestPoint(image, cInfo);
    pair<int,int> bp = brightestPoint.first;
    NormalVector ret;
    ret.x = (brightestPoint.first.first - cInfo.x);
    ret.y = (brightestPoint.first.second - cInfo.y);
    ret.z = -sqrt(pow(cInfo.radius,2) - pow(ret.x,2) - pow(ret.y,2)); //negative value
    //normalization of the vector into a unit vector
    double normalizationConstant = sqrt(pow(ret.z,2) + pow(ret.x,2) + pow(ret.y,2)) / brightestPoint.second; //multiply by the brightness
    ret.x /= normalizationConstant;
    ret.y /= normalizationConstant;
    ret.z /= normalizationConstant;
    //DrawLine(bp.first, bp.second, (bp.first + ret.x), (bp.second + ret.y), 0, &image);
    return ret;
}

//(x – h)2 + (y – k)2 = r2,
pair<pair<int,int>,int> getBrightestPoint(Image & image, const CircleInfo & cInfo) {
    pair<pair<int,int>,int> brightestPoint = {{0,0},0}; // {{x,y},brightness}
    const int & x = cInfo.x, y = cInfo.y, r = cInfo.radius;
   // cout << "using  " << cInfo.x << " " << cInfo.y << " " << cInfo.radius << endl;
    for (int i = y-r; i < y+r; i++) { //i is c
        for (int j = x; pow(j-x,2) + pow(i-y,2) <= pow(r,2); j--) { //j is r
            if(brightestPoint.second < image.GetPixel(j,i)) {
                brightestPoint = {{j,i},image.GetPixel(j,i)}; //update it
            }
        }
        for (int j = x+1; pow(j-x,2) + pow(i-y,2) <= pow(r,2); j++) {
            if(brightestPoint.second < image.GetPixel(j,i)) {
                brightestPoint = {{j,i},image.GetPixel(j,i)}; //update it
            }
        }
    }
    return brightestPoint;
}

void openImageFile(string path, Image & image) {
    if (!ReadImage(path, &image)) {
		cout << "Can't open file " << path << endl;
        abort();
	}
}