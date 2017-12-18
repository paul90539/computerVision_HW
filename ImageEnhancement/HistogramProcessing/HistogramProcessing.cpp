#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int num = 0;

void plotpic(Mat& pic);

/**  @function main */
int main(int argc, char** argv)
{
	Mat src, dst;

	char* source_window = "Source image";
	char* equalized_window = "Equalized Image";

	if (argv[1] == nullptr){
		cout << "ues likes hw4_2.exe <filename>" << endl;
		getchar();
		return 1;
	}
	/// Load image
	src = imread(argv[1], 1);

	if (!src.data)
	{
		cout << "Usage: ./Histogram_Demo <path_to_image>" << endl;
		return -1;
	}
	/// Convert to grayscale
	cvtColor(src, src, CV_BGR2GRAY);

	/// Apply Histogram Equalization
	equalizeHist(src, dst);

	plotpic(src);
	plotpic(dst);

	/// Display results
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	namedWindow(equalized_window, CV_WINDOW_AUTOSIZE);

	imshow(source_window, src);
	imshow(equalized_window, dst);

	/// Wait until user exits the program
	waitKey(0);

	return 0;
}

//plot image Histogram 
void plotpic(Mat& pic){
	
	Mat bw_hist;

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;

	calcHist(&pic, 1, 0, Mat(), bw_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for R, G and B
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8U, Scalar(0));

	normalize(bw_hist, bw_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(bw_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(bw_hist.at<float>(i))),
			Scalar(255), 2, 8, 0);
	}

	char name[1000];
	sprintf(name ,"calcHist_%d", num);
	namedWindow(name, CV_WINDOW_AUTOSIZE);
	imshow(name, histImage);

	num++;
	//waitKey(0);
}