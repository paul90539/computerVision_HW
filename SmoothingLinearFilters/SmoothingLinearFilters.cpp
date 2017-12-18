//opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

#define line 200  /* set thresholding value */ 

//global variables
Mat frame; //current frame
Mat fgMaskMOG; //fg mask generated by MOG method
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
//Mat fgMaskMOG3; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
//BackgroundSubtractorMOG pMOG3; //MOG2 Background subtractor
int keyboard;

//function declarations
void help();
void processVideo(char* videoFilename);
void processImages(char* firstFrameFilename);
Mat& thresholding(Mat& img, int dividline);

void help()
{
	cout
		<< "--------------------------------------------------------------------------" << endl
		<< "This program shows how to use background subtraction methods provided by " << endl
		<< " OpenCV. You can process both videos (-vid) and images (-img)." << endl
		<< endl
		<< "Usage:" << endl
		<< "./bs {-vid <video filename>|-img <image filename>}" << endl
		<< "for example: ./bs -vid video.avi" << endl
		<< "or: ./bs -img /data/images/1.png" << endl
		<< "--------------------------------------------------------------------------" << endl
		<< endl;
}

int main(int argc, char* argv[])
{
	//print help information
	help();

	//check for the input parameter correctness
	if (argc != 3) {
		cerr << "Incorret input list" << endl;
		cerr << "exiting..." << endl;
		return EXIT_FAILURE;
	}

	//create GUI windows
	namedWindow("Frame");
	namedWindow("FG Mask MOG");
	namedWindow("FG Mask MOG 2");
	//namedWindow("FG Mask MOG 3");
	//create Background Subtractor objects
	//NOTE HERE!!!!
	pMOG = new BackgroundSubtractorMOG(); //MOG approach
	pMOG2 = new BackgroundSubtractorMOG2(); //MOG2 approach


	if (strcmp(argv[1], "-vid") == 0) {
		//input data coming from a video
		processVideo(argv[2]);
	}
	else if (strcmp(argv[1], "-img") == 0) {
		//input data coming from a sequence of images
		processImages(argv[2]);
	}
	else {
		//error in reading input parameters
		cerr << "Please, check the input parameters." << endl;
		cerr << "Exiting..." << endl;
		return EXIT_FAILURE;
	}
	//destroy GUI windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}

void processVideo(char* videoFilename) {
	//create the capture object
	VideoCapture capture(videoFilename);
	if (!capture.isOpened()){
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	//read input data. ESC or 'q' for quitting
	while ((char)keyboard != 'q' && (char)keyboard != 27){
		//read the current frame
		if (!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}
		//update the background model
		//AND HERE!!!
		pMOG->operator()(frame, fgMaskMOG);		
		pMOG2->operator()(frame, fgMaskMOG2);
		//pMOG3(frame, fgMaskMOG3);
		//get the frame number and write it on the current frame

		/* make linear filtering + thresholding */
		GaussianBlur(fgMaskMOG, fgMaskMOG, Size(15, 15), 0, 0);
		GaussianBlur(fgMaskMOG2, fgMaskMOG2, Size(15, 15), 0, 0);

		fgMaskMOG = thresholding(fgMaskMOG, line);	/* call thresholding function */
		fgMaskMOG2 = thresholding(fgMaskMOG2, line);
		/* ==================================== */

		stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		ss << capture.get(CV_CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		//show the current frame and the fg masks
		imshow("Frame", frame);
		imshow("FG Mask MOG", fgMaskMOG);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		//imshow("FG Mask MOG 3", fgMaskMOG3);
		//get the input from the keyboard
		keyboard = waitKey(30);
	}
	//delete capture object
	capture.release();
}

void processImages(char* fistFrameFilename) {
	//read the first file of the sequence
	frame = imread(fistFrameFilename);
	if (!frame.data){
		//error in opening the first image
		cerr << "Unable to open first image frame: " << fistFrameFilename << endl;
		exit(EXIT_FAILURE);
	}
	//current image filename
	string fn(fistFrameFilename);
	//read input data. ESC or 'q' for quitting
	while ((char)keyboard != 'q' && (char)keyboard != 27){
		//update the background model
		//ALSO HERE!!!!
		pMOG->operator()(frame, fgMaskMOG);
		pMOG2->operator()(frame, fgMaskMOG2);
		//get the frame number and write it on the current frame
		size_t index = fn.find_last_of("/");
		if (index == string::npos) {
			index = fn.find_last_of("\\");
		}
		size_t index2 = fn.find_last_of(".");
		string prefix = fn.substr(0, index + 1);
		string suffix = fn.substr(index2);
		string frameNumberString = fn.substr(index + 1, index2 - index - 1);
		istringstream iss(frameNumberString);
		int frameNumber = 0;
		iss >> frameNumber;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		//show the current frame and the fg masks
		imshow("Frame", frame);
		imshow("FG Mask MOG", fgMaskMOG);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		//get the input from the keyboard
		keyboard = waitKey(30);
		//search for the next image in the sequence
		ostringstream oss;
		oss << (frameNumber + 1);
		string nextFrameNumberString = oss.str();
		string nextFrameFilename = prefix + nextFrameNumberString + suffix;
		//read the next frame
		frame = imread(nextFrameFilename);
		if (!frame.data){
			//error in opening the next image in the sequence
			cerr << "Unable to open image frame: " << nextFrameFilename << endl;
			exit(EXIT_FAILURE);
		}
		//update the path of the current frame
		fn.assign(nextFrameFilename);
	}
}

//threshold to decide pixel color is 0 or 255
Mat& thresholding(Mat& img, int dividline){
	
	int row = img.rows;
	int col = img.cols;
	int channel = img.channels();
	uchar* p = img.data;

	if (channel == 1){
		for (int i = 0; i < row * col; i++, p++){
			if (*p > dividline) *p = 255;
			else *p = 0;
		}
	}
	return img;	
}