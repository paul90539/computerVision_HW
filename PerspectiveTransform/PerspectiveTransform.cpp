#include "opencv2/highgui/highgui.hpp"
#include<opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include<vector>

using namespace cv;
using namespace std;

Mat img;
Mat star;

vector< Point2f > pointarray(10);

void onMouse(int Event, int x, int y, int flags, void* param);
void PerspectiveTransform();

int main(int argc, char* argv[])
{
	if (argv[1] != nullptr) star = imread(argv[1], CV_LOAD_IMAGE_COLOR); // load in argv
	else star = imread("sample.jpg", CV_LOAD_IMAGE_COLOR);
	
	namedWindow("Display window", WINDOW_AUTOSIZE);
	
	pointarray.clear(); // clear vector
		
	img = star.clone();
	imshow("Display window", img);
	setMouseCallback("Display window", onMouse, NULL);	// mouse listener
	waitKey(0);

	return 0;

}

void onMouse(int Event, int Orix, int Oriy, int flags, void* param){ // mouse performed

	if (Event == CV_EVENT_LBUTTONDOWN){

		circle(img, Point(Orix, Oriy), 5, Scalar(0, 0, 255), -1); // if click create red points, Point need int value
		imshow("Display window", img);
		
		float x = (float) Orix, y = (float) Oriy; // Point2f need float value
		
		if (pointarray.size() == 0){ // save click site
			pointarray.push_back(Point2f(x, y));
		}
		else if (pointarray.size() == 1){
			pointarray.push_back(Point2f(x, y));
		}
		else if (pointarray.size() == 2){
			pointarray.push_back(Point2f(x, y));
		}
		else if (pointarray.size() == 3){ // if click four times, trans picture
			pointarray.push_back(Point2f(x, y));
			PerspectiveTransform();
		}
		else if (pointarray.size() == 4){
			pointarray.clear();
		}			
	}
}

void PerspectiveTransform(){ // picture trans
	// Input Quadilateral or Image plane coordinates
	Point2f inputQuad[4];
	// Output Quadilateral or World plane coordinates
	Point2f outputQuad[4];

	// The 4 points that select quadilateral on the input , from top-left in clockwise order
	// These four pts are the sides of the rect box used as input 
	inputQuad[0] = pointarray.at(0);
	inputQuad[1] = pointarray.at(1);
	inputQuad[2] = pointarray.at(2);
	inputQuad[3] = pointarray.at(3);
	// The 4 points where the mapping is to be done , from top-left in clockwise order
	outputQuad[0] = Point2f(0, 0);
	outputQuad[1] = Point2f( (float) (img.cols - 1), 0 );
	outputQuad[2] = Point2f( (float) (img.cols - 1), (float) (img.rows - 1) );
	outputQuad[3] = Point2f( 0, (float) (img.rows - 1) );

	Mat lambda(2, 4, CV_32FC1);

	//Mat output;
	Mat output;

	lambda = Mat::zeros(img.rows, img.cols, img.type());

	// Get the Perspective Transform Matrix i.e. lambda 
	lambda = getPerspectiveTransform(inputQuad, outputQuad);
	// Apply the Perspective Transform just found to the src image
	warpPerspective(star, output, lambda, output.size());

	namedWindow("Output", WINDOW_AUTOSIZE);
	imshow("Output", output);

	pointarray.clear(); // reset
	img = star.clone();
	imshow("Display window", img);
	waitKey(0);
}
