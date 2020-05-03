#include <project_360_visual.h>
#include <projection.h>
#include <iostream>

template <typename T> string tostr(T&  t) {
	ostringstream o;
	o << t;
	return o.str();
}

void colVec(Mat m, vector<double>& a) {
	Mat temp = m.reshape(1, m.total() * m.channels());
	if (m.isContinuous()) {
		a = temp;
	}
	else a = temp.clone();
}

void vecToMat(vector<double> i1, vector<double> i2, vector<double> i3, vector<double> i4, vector<double> i5, vector<double> i6, Mat& M) {
	M = Mat::zeros(Size(6,i1.size()), CV_64F);
	for (int i = 0; i < i1.size(); i++) {
		M.at<double>(0, i) = i1[i];
		M.at<double>(1, i) = i2[i];
		M.at<double>(2, i) = i3[i];
		M.at<double>(3, i) = i4[i];
		M.at<double>(4, i) = i5[i];
		M.at<double>(5, i) = i6[i];
	}
}

void vecToMat(vector<double> i1, Mat& M) {
	M = Mat::zeros(Size(1, i1.size()),CV_64F);
	for (int i = 0; i < i1.size(); i++) {
		M.at<double>(0, i) = i1[i];
	}
}

int main(int argc, char **argv){

	cout << "OpenCV version: " << CV_VERSION << endl;

	
	string input1(argv[1]);;


	namedWindow("SHW", WINDOW_AUTOSIZE);
	namedWindow("BCK0", WINDOW_AUTOSIZE);
	namedWindow("BCK1", WINDOW_AUTOSIZE);
	moveWindow("BCK0", 650, 50);
	moveWindow("BCK1", 50,200);
	namedWindow("BCK2", WINDOW_AUTOSIZE);
	moveWindow("BCK2",50,500);

	Mat frame, out, back;

	VideoCapture capture(input1);

	capture >> frame;
	int height = frame.rows; // altezza frame
	int width = frame.cols; // larghezza frame

		// angoli di eulero e zoom
	double rp = 155.0;
//	double rp = -95.0;
//	double rp = -85.0;
//	double rp = 240.0;
//	double rp = 230.0;
//	double rp = 70.0;
//	double rp = 60.0;
	double rt = -25.0;   
//	double rp = -65.0;
//	double rt = -37.0;
	double rr = 0.0;     
//	double zoom = 350;     // piu' è alto, piu' ravvicinata è l'immagine.
	double zoom = 300;

	double var = 2.0;



	VideoWriter vw("out.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 14, Size(500, 300));
	Rect2d bbox;
	Ptr<Tracker> tracker = TrackerCSRT::create(); // Tracker: CSRT, MOSSE, KCF, GOTURN
	
	bool fromcenter = false;

	image_projection(frame, out, rp, rt, zoom);

	bbox = selectROI("SHW", out, fromcenter);
	tracker->init(out, bbox);

//  OFFLINE

	Mat t1, tt1, tt2, tt3, tt4, tt5, tt6, temp1, temp2, temp3, temp4, temp5, temp6;
	t1 = out.clone();
	
	image_projection(frame, tt1, rp - var, rt, zoom);     // spostamento a sx
	image_projection(frame, tt2, rp + var, rt, zoom);     // spostamento a dx
	image_projection(frame, tt3, rp, rt - var, zoom);     // spostamento in basso
	image_projection(frame, tt4, rp, rt + var, zoom);     // spostamento in alto
	image_projection(frame, tt5, rp, rt, zoom - var);     // zoom out
	image_projection(frame, tt6, rp, rt, zoom + var);     // zoom in



	Mat gt1, gtt1, gtt2, gtt3, gtt4, gtt5, gtt6;
	cvtColor(t1(bbox), gt1, COLOR_BGR2GRAY);	
	cvtColor(tt1(bbox), gtt1, COLOR_BGR2GRAY);
	cvtColor(tt2(bbox), gtt2, COLOR_BGR2GRAY);
	cvtColor(tt3(bbox), gtt3, COLOR_BGR2GRAY);
	cvtColor(tt4(bbox), gtt4, COLOR_BGR2GRAY);
	cvtColor(tt5(bbox), gtt5, COLOR_BGR2GRAY);
	cvtColor(tt6(bbox), gtt6, COLOR_BGR2GRAY);

	temp1 = gt1 - gtt1;
	temp2 = gt1 - gtt2;
	temp3 = gt1 - gtt3;
	temp4 = gt1 - gtt4; 
	temp5 = gt1 - gtt5;
	temp6 = gt1 - gtt6;


	imshow("BCK1", temp1);
	imshow("BCK2", temp2);

//	tracker->init(out,bbox);

	vector<Vec3d> w = { {-var,var},{-var,var},{-var,var}};  // w = [wp, wt, wz]
	vector<double> i1, i2, i3, i4, i5, i6;
	Mat A, At, AtA, AtI, x;

	cout << "righe: " << temp1.rows << endl;
	cout << "colonne: " << temp1.cols << endl;
	cout << "channels: " << temp1.channels() << endl;

	colVec(temp1, i1);
	colVec(temp2, i2);
	colVec(temp3, i3);
	colVec(temp4, i4);
	colVec(temp5, i5);
	colVec(temp6, i6);

	vecToMat(i1,i2,i3,i4, i5, i6, A);

	transpose(A, At);

	
	AtA = At * A;
//	gemm(At,A, 1.0, vector<double>(), 0.0, AtA);
	invert(AtA,AtI);
	
	x = AtI * At;

// END OFFLINE

	Mat sample, sample2, outg, prvg, c_sample, ss;
	vector<double> s, samplec;
	double sp, st, sz;
	Mat prv = out.clone();


	while (true) {

		double timer = (double)getTickCount();
		capture >> frame;

		if (frame.empty()) break;


		image_projection(frame, out, rp, rt, zoom);

	
		cvtColor(out, outg, CV_BGR2GRAY);
		cvtColor(prv, prvg, CV_BGR2GRAY);
		back = outg - prvg;

		
		prv = out.clone();

		sample = back(bbox);
		resize(sample, sample, Size(gt1.cols,gt1.rows));          // solo tracking CSRT
		c_sample = sample.clone();                               // rende sample continua
	
		colVec(c_sample, s);
		vecToMat(s, ss);
		sample2 = x * ss;
		colVec(sample2, samplec);

		sp = samplec[0]*w[0][0] + samplec[1]*w[0][1];
		st = samplec[2]*w[1][0] + samplec[3]*w[1][1]; 
		sz = samplec[4]*w[2][0] + samplec[5]*w[2][1];

		cout << "pan var: " << sp << endl;
		cout << "tilt var: " << sp << endl;
		cout << "zoom: " << sz << endl;
		
		rp = rp + sp;
		rt = rt + st;
		zoom = zoom + sz;

		bool ok = tracker->update(out, bbox);                        // aggiorna il rettangolo attorno all'oggetto

		if(!ok){
		//	putText(out, "fail", Point(20, 130), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
		}
	//	else putText(out, "ok", Point(20, 130), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
	
		

		rectangle(out, bbox, Scalar(255,0,0));                      // disegna il rettangolo attorno all'oggetto


		float fps = getTickFrequency() / ((double)getTickCount() - timer);

	//	putText(out, "fps: " + tostr(fps), Point(20,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170, 50), 2);
	//	putText(out, "rp: " + tostr(rp), Point(20, 70), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
	//	putText(out, "rt: " + tostr(rt), Point(20, 90), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
	//	putText(out, "zoom: " + tostr(zoom), Point(20, 110), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		imshow("SHW", out);
		imshow("BCK0", sample);
		vw.write(out);

		int keyboard = waitKey(1);

		if (keyboard == 'q' || keyboard == 27) break;
	}
}

