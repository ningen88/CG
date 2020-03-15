#include "projection.h"

Mat rotmat(double x, double y) {

	Mat R;
	Mat Rx = (Mat_<double>(3, 3) <<
		1, 0, 0,                        //rotazione lungo asse x
		0, cos(x), -sin(x),
		0, sin(x), cos(x));

	Mat Ry = (Mat_<double>(3, 3) <<
		cos(y), 0, sin(y),              //rotazione lungo asse y
		0, 1, 0,
		-sin(y), 0, cos(y));

	R = Ry * Rx;

	return R;
}

double* projection(int x_view, int y_view, Mat R, int width, int height, Mat K) {

	Mat xyz, xyz_norm, RK, ray3d;
	double x, y, z, theta, phi;
	static double out[2];

	xyz = (Mat_<double>(3, 1) << (double)x_view, (double)y_view, 1);

	xyz_norm = xyz / norm(xyz);
	RK = R * K.inv();
	ray3d = RK * xyz_norm;

	x = ray3d.at<double>(0, 0);
	y = ray3d.at<double>(0, 1);
	z = ray3d.at<double>(0, 2);

//	theta = atan2(y, sqrt(x * x + z * z));
//	phi = atan2(x, z);

	phi = atan2(y, sqrt(x * x + z * z));
	theta = atan2(x, z);


//	out[0] = (((phi * width) / M_PI + width) / 2);
//	out[1] = (theta + M_PI / 2) * height / M_PI;

	out[0] = (((theta * width) / M_PI + width) / 2);
	out[1] = (phi + M_PI / 2) * height / M_PI;

	return out;
}

void image_projection(Mat eq, Mat& out, double rp, double rt, double f) {

	double pan = (rp * M_PI) / 180;
	double tilt = (rt * M_PI) / 180;

	double view_w = 300;
	double view_h = 500;

	Mat viewport = Mat::zeros(Size(view_h, view_w), CV_8UC3);

	double w = eq.cols;
	double h = eq.rows;
	double x, y, dx, dy, w1, w2, w3, w4;
	double near_x, near_y;
	double* position;
	Vec3b color;
	Mat R = rotmat(tilt, pan);
	Mat K = (Mat_<double>(3, 3) <<
		f, 0, view_w / 2,
		0, f, view_h / 2,
		0, 0, 1);

	for (int v = 0; v < view_w; v++) {
		for (int u = 0; u < view_h; u++) {

			position = projection(u, v, R, w, h, K);

			x = position[0];
			y = position[1];

			near_x = int(x);
			near_y = int(y);

			if (x < 0 || y < 0 || near_x > w - 1 || near_y > h - 1) {
				continue;
			}

			dx = x - near_x;
			dy = y - near_y;

			w1 = (1.0 - dx) * (1.0 - dy);
			w2 = dx * (1.0 - dy);
			w3 = dy * (1.0 - dx);
			w4 = dx * dy;

			color = w1 * eq.at<Vec3b>(near_y, near_x) + w2 * eq.at<Vec3b>(near_y, near_x + 1) + w3 * eq.at<Vec3b>(near_y + 1, near_x) + w4 * eq.at<Vec3b>(near_y + 1, near_x + 1);

			viewport.at<Vec3b>(Point(u, v)) = color;

		//	viewport.at<Vec3b>(Point(u, v)) = eq.at<Vec3b>(near_y, near_x);

		}
	}

	out = viewport.clone();

}