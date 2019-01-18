/*
    Allan Spektor S3
    Given the matrix S from part 2, find the inverse and then multiply by the irradiance to get the surface normal
    Then normalize the resultant surface normal vector and draw the line for the vector on image 1.
    Only draw the line every certain number of steps and only for points above a specific threshold

    *note* relies on a modified image.cc (the copy constructor breaks when invoked because pixels_ is not set to nullptr)
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
    struct matrix3x3 {
        double m[3][3] = {{0}};
        matrix3x3(){}
        matrix3x3(vector<NormalVector> & v) {loadFrom3NormalVector(v);}
        //loads matrix
        double & operator()(int i, int j) {
            if(i >= 0 && j >= 0 && i < 3 && j < 3) {
                return m[i][j];
            } else {
                cout << "accessing matrix out of bounds (" << i << ", " << j << ")" << endl;
                return m[0][0];
            }
        }
        void loadFrom3NormalVector(vector<NormalVector> & v) {
            for(int i = 0; i < 3; i++) {
                m[i][0] = v[i].x;
                m[i][1] = v[i].y;
                m[i][2] = v[i].z;
            }
        } 
        void print() {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    cout << m[i][j] << " ";
                }
                cout << endl;
            }
        }
    };
}

matrix3x3 calcInverseMatrix(matrix3x3 & m);
void openImageFile(string path, Image & image);
bool checkThreshold(vector<Image> & images, int threshold, int r, int c);
NormalVector generateIrradianceVector(vector<Image> & images, int r, int c);
NormalVector multiplyInverseByIrradiance(matrix3x3 & inverseMatrix, NormalVector irradiance);
Image computeSurfaceNormals(vector<Image> & images, matrix3x3 & inverseMatrix, int step, int threshold);


int main(int argc, char **argv) 
{

	if (argc != 8) {
		printf("Usage: %s {input directions} {image 1} {image 2} {image 3} {step} {threshold} {output}\n", argv[0]);
		return -1;
	}

	const string input_directions(argv[1]); // [3] x,y,z normalized + scaled vector
    const vector<string> imageFilePaths = {argv[2], argv[3], argv[4]};
    const int step = atoi(argv[5]);
    const int threshold = atoi(argv[6]);
	const string output_file(argv[7]);

    vector<Image> images(3);
    vector<NormalVector> directions(3);
    ifstream iFile(input_directions);

    //loads direction info
    if(!iFile.fail()) {
        for(auto & i : directions) {
            iFile >> i.x >> i.y >> i.z;
        }
        iFile.close();
    } else {
        cout << "Failed opening " << input_directions << endl;
        return -1;
    }

    //loads the 3 pgm files into image
    for(int i = 0; i < 3; i++) {
        openImageFile(imageFilePaths[i], images[i]);
    }

    matrix3x3 normalMatrix(directions);
    matrix3x3 inverseMatrix = calcInverseMatrix(normalMatrix);

    Image output = computeSurfaceNormals(images, inverseMatrix, step, threshold);
    
    if(!WriteImage(output_file, output)) {
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

	cout << "done" << endl;
}

Image computeSurfaceNormals(vector<Image> & images, matrix3x3 & inverseMatrix, int step, int threshold) {
    Image ret = images[0]; //makes a copy of 1
    for(int i = 0; i < images[0].num_rows(); i++) {
        for(int j = 0; j < images[0].num_columns(); j++) {
            if(checkThreshold(images, threshold, i, j)) {
                if(i % step == 0 && j % step == 0) { //checks for step
                    NormalVector surfaceNormal = multiplyInverseByIrradiance(inverseMatrix, generateIrradianceVector(images, i, j));
                    double normalizationConstant = sqrt(pow(surfaceNormal.x,2) + pow(surfaceNormal.y,2) + pow(surfaceNormal.z,2)) / 10;
                    surfaceNormal.x /= normalizationConstant; //normalize the normal
                    surfaceNormal.y /= normalizationConstant;
                    surfaceNormal.z /= normalizationConstant;
                    DrawLine(i, j, (i + surfaceNormal.x), (j + surfaceNormal.y), 255, &ret);
                    ret.SetPixel(i, j, 0);
                }
            }
        }
    }
    return move(ret); //is this smart?
}

//computes the inverse of a matrix m
//credit to Cornstalks from S.O. for algorithm to invert 3x3 matrix
matrix3x3 calcInverseMatrix(matrix3x3 & m) {
    double det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
                 m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
                 m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
    
    if(det == 0) {
        cout << "determinant is 0, returning blank matrix" << endl;
        return matrix3x3();
    }

    double invdet = 1 / det;

    matrix3x3 minv; // inverse of matrix m
    minv(0, 0) = (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) * invdet;
    minv(0, 1) = (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2)) * invdet;
    minv(0, 2) = (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * invdet;
    minv(1, 0) = (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * invdet;
    minv(1, 1) = (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * invdet;
    minv(1, 2) = (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2)) * invdet;
    minv(2, 0) = (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * invdet;
    minv(2, 1) = (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) * invdet;
    minv(2, 2) = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * invdet;

    return minv;
}

NormalVector generateIrradianceVector(vector<Image> & images, int r, int c) {
    NormalVector ret;
    ret.x = images[0].GetPixel(r,c);
    ret.y = images[1].GetPixel(r,c);
    ret.z = images[2].GetPixel(r,c);
    return move(ret);
}

//returns the mining 3x1 matrix from 3x3 * 3x1
NormalVector multiplyInverseByIrradiance(matrix3x3 & inverseMatrix, NormalVector irradiance) {
    auto mult = [&](int i)->double {
        return inverseMatrix(i,0) * irradiance.x +
               inverseMatrix(i,1) * irradiance.y +
               inverseMatrix(i,2) * irradiance.z; 
    };
    return NormalVector{mult(0), mult(1), mult(2)};
}

bool checkThreshold(vector<Image> & images, int threshold, int r, int c) {
    for(int i = 0; i < images.size(); i++) {
        if(images[i].GetPixel(r,c) < threshold) return false;
    }
    return true;
}

void openImageFile(string path, Image & image) {
    if (!ReadImage(path, &image)) {
		cout << "Can't open file " << path << endl;
        abort();
	}
}