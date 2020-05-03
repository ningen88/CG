#pragma once

#ifndef _Fun
#define _Fun
#endif 

#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <cmath>


using namespace std;
using namespace cv;


void colVec(Mat m, vector<double>& a);
void vecToMat(vector<double> i1, vector<double> i2, vector<double> i3, vector<double> i4, Mat& M);
void vecToMat(vector<double> i1, Mat& M);