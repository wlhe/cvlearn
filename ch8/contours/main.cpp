#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void sampleContours(const std::string &imgname)
{
    Mat src = imread(imgname, IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat dst = Mat::zeros(src.size(), CV_8UC3);
    src = src > 120;
    imshow("Src > 120", src);

    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;

    findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    int i = 0;
    for (; i >= 0; i = hierarchy[i][0]) {
        Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
        drawContours(dst, contours, i, color, 1, LINE_AA, hierarchy);
    }

    imshow("Result", dst);
}

Mat src, gray, dst, cannyOut;
int nthreshold = 10;
RNG rng(12333);
static void onThreshold(int, void *)
{
    Canny(gray, cannyOut, nthreshold, nthreshold * 2, 3);
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(cannyOut, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros(cannyOut.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 1, LINE_AA, hierarchy);
    }
    imshow("Contours", drawing);
}
static void contoursDemo(const std::string &imgname)
{
    src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    cvtColor(src, gray, COLOR_BGR2GRAY);
    blur(gray, gray, Size(3, 3));

    namedWindow("Contours");
    imshow("Gray", gray);

    createTrackbar("Threshold", "Contours", &nthreshold, 255, onThreshold);
    onThreshold(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleContours(imgname);

    contoursDemo(imgname);

    waitKey(0);
    return 0;
}