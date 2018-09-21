#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

static void imgShow()
{
    Mat img = imread("images/1.jpg");
    Mat img2 = imread("images/1.jpg", IMREAD_GRAYSCALE);
    imshow("Picture", img);
    imshow("Gray", img2);
}

static void imgErosion()
{
	Mat img = imread("images/2.jpg");
	imshow("Origin", img);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	erode(img, dstImage, element);
	imshow("Erode", dstImage);
}

static void imgBlur()
{
    Mat img = imread("images/3.jpg");
    imshow("Origin", img);

    Mat dstImage;
    blur(img, dstImage, Size(7, 7));

    imshow("Blur", dstImage);
}

static void imgCanny()
{
    Mat img = imread("images/4.jpg");
    imshow("Origin", img);

    Mat edge, grayImage;

    cvtColor(img, grayImage, COLOR_BGR2GRAY);

    blur(grayImage, edge, Size(3, 3));

    Canny(edge, edge, 3, 9, 3);

    imshow("Canny", edge);
}

static void videoPlay()
{
    VideoCapture cap("images/1.avi");

    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        imshow("Video", frame);
        waitKey(30);
    }
}

static void videoCapture()
{
    VideoCapture cap(0);

    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        imshow("Capture", frame);
        waitKey(30);
    }
}

static void videoCanny()
{
    VideoCapture cap(0);

    Mat frame;
    Mat edge, grayImage;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        cvtColor(frame, grayImage, CV_BGR2GRAY);

        blur(grayImage, edge, Size(3, 3));

        Canny(edge, edge, 0, 30, 3);

        imshow("CapOrigin", frame);
        imshow("CapCanny", edge);
        waitKey(30);
    }
}

int main()
{
    //imgShow();

	//imgErosion();

    //imgBlur();

    //imgCanny();

    //videoPlay();

    //videoCapture();

    videoCanny();

	waitKey(0);
	return 0;
}