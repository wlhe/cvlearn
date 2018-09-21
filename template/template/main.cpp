#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
	Mat img = imread("test.jpg");
	imshow("Picture", img);
	waitKey(0);
	return 0;
}