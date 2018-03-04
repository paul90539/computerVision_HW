#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	VideoCapture cap(0); //open the video camera no. 0 #### or open the video in the address
	char name[100];	//saved the files or video name
	int mode = 0; //to check video recording or not
	SYSTEMTIME stLocal;	//get systemtime
	GetLocalTime(&stLocal);

	if (!cap.isOpened()) //if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		getchar();
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	Size videoSize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)); // get video size
	VideoWriter writer; // for video recording

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	double delayTime = 30; //set videoCap delayTime
	//fps = cap.get(CV_CAP_PROP_FPS);  //get video fps

	while (1)
	{
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video
		int intcom; // listener command
		intcom = waitKey((int)delayTime); // get command and set waiting time(ms) 
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		writer.write(frame);
		imshow("MyVideo", frame); //show the frame in "MyVideo" window

		if (intcom == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

		if (intcom == 'z') //wait for 'z' key press for 30ms. This function is save picture.
		{
			GetLocalTime(&stLocal);
			sprintf_s(name, "%d_%d_%d_%d_%d_%d.jpg", stLocal.wYear, stLocal.wMonth, //set name
				stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
			printf("%s\n", name);
			imwrite(name, frame);	//picture save
		}
		if (intcom == 'a' && mode == 0) { //wait for 'a' key press for 30ms. This function is start record video.
			mode = 1;
			GetLocalTime(&stLocal);
			sprintf_s(name, "%d_%d_%d_%d_%d_%d.avi", stLocal.wYear, stLocal.wMonth, //set name
				stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
			printf("%s\n", name);
			writer.open(name, CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize); //start recording to video
		}
		if (mode == 1) { //This function is recording loop.
			writer.write(frame); //video recording loop
		}
		if (intcom == 's') { //wait for 's' key press for 30ms. This function is end record video.
			writer.release(); //end recording to video
			mode = 0;
		}
	}
	return 0;
}