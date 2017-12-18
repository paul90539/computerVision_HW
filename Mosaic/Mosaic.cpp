//#include "opencv2/highgui/highgui.hpp"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

// Mat& showpicture(Mat& star, int size) // if want return value, header need address sign '&'

 // show Mosaic with point (The efficient way)
void showPoint(Mat& sourceImage, int size){

	double t = (double)getTickCount(); // set time ticket
	// do something ...

	int channels = sourceImage.channels(); // BW is 1 and BGR is 3
	int nrow = sourceImage.rows; // get picture's row
	int ncol = sourceImage.cols * channels; // get picture's col
	int newValue[3] = { 0 }; // set color change value
	int sitecol = 0, siterow = 0; // get current site
	uchar* p = sourceImage.data;

	while (siterow < nrow){ // if siterow at picture outside ,break loop
		newValue[0] = 0; // init
		newValue[1] = 0;
		newValue[2] = 0;

		int endrow, endcol; // site for where need to stop in this loop

		if (sitecol + size * channels > ncol) endcol = (ncol - sitecol) / channels;
		else endcol = size; // set endcol

		if (siterow + size > nrow) endrow = nrow - siterow;
		else endrow = size; // set endcol
		//printf("%d,%d\n", sitecol, siterow);

		for (int colortype = 0; colortype < channels; colortype++){ // get new value for Mosaic
			for (int j = 0; j < endrow; j++){
				for (int k = 0; k < endcol; k++){
					newValue[colortype] += *(p + ncol * j + k * channels + colortype);
				}
			}
			newValue[colortype] /= endrow * endcol; // value average
		}
		for (int colortype = 0; colortype < channels; colortype++){ // set new value in old picture
			for (int j = 0; j < endrow; j++){
				for (int k = 0; k < endcol; k++){
					*(p + ncol * j + k * channels + colortype) = newValue[colortype];
				}
			}
		}
		p = p + endcol*channels; // current point move
		sitecol += endcol*channels; // sitecol move
		if (sitecol == ncol){ // if sitecol to picture's col end, change to next row
			p = p + ncol * (size - 1);
			sitecol = 0;
			siterow += size;
		}
		
	}
	namedWindow("windowPoint", WINDOW_AUTOSIZE);
	imshow("windowPoint", sourceImage);
	// do something ...
	t = ((double)getTickCount() - t) / getTickFrequency(); // show excution time
	cout << "Point Times passed in seconds: " << t << endl;
	//waitKey(0);
}

// show Mosaic with at (On-the-fly address calculation with reference returning)
void showAt(Mat& sourceImage, int size){ 

	double t = (double)getTickCount(); // set time ticket
	// do something ...

	int channels = sourceImage.channels(); // BW is 1 and BGR is 3
	int nrow = sourceImage.rows; // get picture's row
	int ncol = sourceImage.cols; // get picture's col
	int newValue[3] = { 0 }; // set color change value
	int sitecol = 0, siterow = 0; // get current site

	Mat_<Vec3b> sourceImageAt = sourceImage; 

	while (siterow < nrow){ // if siterow at picture outside ,break loop

		newValue[0] = 0; // init
		newValue[1] = 0;
		newValue[2] = 0;

		int endrow, endcol; // site for where need to stop in this loop

		if (sitecol + size > ncol) endcol = ncol - sitecol;
		else endcol = size; // set endcol

		if (siterow + size > nrow) endrow = nrow - siterow;
		else endrow = size; // set endrow

		for (int j = siterow; j < siterow + endrow; j++){ // get new value for Mosaic
			for (int k = sitecol; k < sitecol + endcol; k++){
				newValue[0] += sourceImageAt(j, k)[0];
				newValue[1] += sourceImageAt(j, k)[1];
				newValue[2] += sourceImageAt(j, k)[2];
			}
		}

		newValue[0] /= endrow * endcol;  // value average
		newValue[1] /= endrow * endcol;
		newValue[2] /= endrow * endcol;

		for (int j = siterow; j < siterow + endrow; j++){  // set new value in old picture
			for (int k = sitecol; k < sitecol + endcol; k++){
				//printf("after = %d, before = %d\n", sourceImageAt(j, k)[2], newValue[2]);
				sourceImageAt(j, k)[0] = newValue[0];
				sourceImageAt(j, k)[1] = newValue[1];
				sourceImageAt(j, k)[2] = newValue[2];				
			}
			//getchar();
		}

		sitecol += endcol; // sitecol move
		if (sitecol == ncol){ // if sitecol to picture's col end, change to next row
			sitecol = 0;
			siterow += size;
		}
	}

	sourceImage = sourceImageAt;

	namedWindow("windowAt", WINDOW_AUTOSIZE);
	imshow("windowAt", sourceImage);
	// do something ...
	t = ((double)getTickCount() - t) / getTickFrequency(); // show excution time
	cout << "At Times passed in seconds: " << t << endl;
	//waitKey(0);
}

int main(int argc, char* argv[])
{
	Mat originImage = imread("sample.jpg", CV_LOAD_IMAGE_COLOR); // origin picture
	Mat imgPoint, imgAt; // pitcture for point and at
	int num; // enlarge

	namedWindow("Display window", WINDOW_AUTOSIZE); // set origin picture window
	imshow("Display window", originImage);
	waitKey(0);
	
	printf("Size : %d X %d\nChannels : %d\n", originImage.rows, originImage.cols, originImage.channels() );
	printf("\nintput number to set enlarge value: ");
	
	while (scanf("%d", &num) != EOF){

		imgPoint = originImage.clone(); // clone picture
		imgAt = originImage.clone();

		showPoint(imgPoint, num);
		showAt(imgAt, num);

		waitKey(0);
		printf("\nintput number to set enlarge value: ");
	}
	return 0;
}