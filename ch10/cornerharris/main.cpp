#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void sampleCornerHarris(const std::string &imgname)
{
    Mat src = imread(imgname, IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat corner;
    cornerHarris(src, corner, 2, 3, 0.01);

    Mat harrisCorner;
    threshold(corner, harrisCorner, 0.00001, 255, THRESH_BINARY);

    imshow("Corner", harrisCorner);
}

static Mat src, src1, gray;
static int thresh = 30, maxThresh = 175;
static void onCornerHarris(int, void *)
{
    Mat dst, norm, scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    src1 = src.clone();

    cornerHarris(gray, dst, 2, 3, 0.04);
    normalize(dst, norm, 0, 255, NORM_MINMAX, CV_32FC1);
    convertScaleAbs(norm, scaled);
    for (int j = 0; j < norm.rows; j++) {
        for (int i = 0; i < norm.cols; i++) {
            if ((int)norm.at<float>(j, i) > thresh + 80) {
                circle(src1, Point(i, j), 5, Scalar(10, 10, 255), 2);
                circle(scaled, Point(i, j), 5, Scalar(10, 10, 255), 2);
            }
        }
    }
    imshow("Corner src", src1);
    imshow("Corner", scaled);
}
static void cornerHarrisDemo(const std::string &imgname)
{
    src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    src1 = src.clone();
    cvtColor(src, gray, COLOR_BGR2GRAY);

    namedWindow("Corner src");
    namedWindow("Corner");

    createTrackbar("Threshold", "Corner src", &thresh, maxThresh, onCornerHarris);
    onCornerHarris(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleCornerHarris(imgname);

    cornerHarrisDemo(imgname);

    waitKey(0);
    return 0;
}