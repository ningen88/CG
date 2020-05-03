#pragma once

#ifndef _Proj
#define _Proj
#endif
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Mat rotmat(double pan, double tilt);

double* projection(int x_view, int y_view, Mat R, int width, int height, Mat K);

void image_projection(Mat eq, Mat& out, double rp, double rt, double f);