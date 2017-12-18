#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int morph_elem = 2;
int morph_size = 9;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

char* window_name = "Morphology Transformations Demo";
void Morphology_Operations(int, void*);
Mat dst;
Mat src, GB, TS, output;

/** @function main */
int main(int argc, char** argv)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int thresh = 100;
	int max_thresh = 255;
	RNG rng(12345);

	/// Load an image
	src = imread("sample.jpg", 0);

	if (!src.data)
	{
		return -1;
	}

	/// Remove noise by blurring with a Gaussian filter
	GaussianBlur(src, GB, Size(19, 19), 0, 0, BORDER_DEFAULT);

	namedWindow("GaussianBlur", CV_WINDOW_AUTOSIZE);
	imshow("GaussianBlur", GB);

	threshold(GB, TS, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	/// Create window
	namedWindow("Threshold", CV_WINDOW_AUTOSIZE);
	imshow("Threshold", TS);



	/// Create window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	/*
	/// Create Trackbar to select Morphology operation
	createTrackbar("Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, Morphology_Operations);

	/// Create Trackbar to select kernel type
	createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
		&morph_elem, max_elem,
		Morphology_Operations);

	/// Create Trackbar to choose kernel size
	createTrackbar("Kernel size:\n 2n +1", window_name,
		&morph_size, max_kernel_size,
		Morphology_Operations);
	*/
	/// Default start
	Morphology_Operations(0, 0);

	/// Detect edges using canny
	Canny(dst, output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	waitKey(0);

	return 0;
}


/**
* @function Morphology_Operations
*/
void Morphology_Operations(int, void*)
{
	// Since MORPH_X : 2,3,4,5 and 6
	int operation = morph_operator + 2;

	Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

	/// Apply the specified morphology operation
	morphologyEx(TS, dst, operation, element);
	imshow(window_name, dst);
}