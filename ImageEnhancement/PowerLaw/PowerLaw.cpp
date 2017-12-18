#include <opencv2\opencv.hpp>
#include <highgui.h>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */
double gamma;

int main(int argc, char** argv)
{
	/// Read image given by user
	if (argv[1] == nullptr){
		cout << "ues likes hw4_1.exe <filename>" << endl;
		getchar();
		return 1;
	}
	Mat image = imread(argv[1]);

	Mat new_image = Mat::zeros(image.size(), image.type());

	//cvtColor(image, image, CV_BGR2GRAY);

	//Initialize values
	std::cout << " Basic Linear Transforms " << std::endl;
	std::cout << "-------------------------" << std::endl;
	std::cout << "* Enter the alpha value [1.0-3.0]: "; std::cin >> alpha;
	std::cout << "* Enter the beta value [0-100]: "; std::cin >> beta;
	std::cout << "* Enter the gamma value [0.0-2.0]: "; std::cin >> gamma;

	//Do the operation new_image(i,j) = alpha*image(i,j) + beta
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				//Enhancement
				new_image.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>( pow( ( ( alpha*(image.at<Vec3b>(y, x)[c]) + beta) / 255 ), gamma ) * 255 );
			}
		}
	}

	/// Create Windows
	namedWindow("Original Image", 1);
	namedWindow("New Image", 1);

	/// Show stuff
	imshow("Original Image", image);
	imshow("New Image", new_image);

	/// Wait until user press some key
	waitKey();
	return 0;
}