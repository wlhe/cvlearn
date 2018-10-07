#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static Mat src, gray;
static int cornerNum = 30, maxCornerNum = 500;
static void onGoodFeaturesToTrack(int, void *)
{
    if (cornerNum < 1) {
        cornerNum = 1;
    }

    std::vector<Point2f> corners;
    Mat copy = src.clone();

    goodFeaturesToTrack(gray, corners, cornerNum, 0.01, 10,
                        Mat(), 3, false, 0.04);

    std::cout << "Corner Number: " << corners.size() << std::endl;

    for (auto &c : corners) {
        circle(copy, c, 3, Scalar(0, 0, 255), 2);
    }
    imshow("Corner", copy);
}
static void goodFeaturesToTrackDemo(const std::string &imgname)
{
    src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    cvtColor(src, gray, COLOR_BGR2GRAY);

    namedWindow("Corner");

    createTrackbar("Corner Num", "Corner", &cornerNum, maxCornerNum, onGoodFeaturesToTrack);
    onGoodFeaturesToTrack(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/3.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    goodFeaturesToTrackDemo(imgname);

    waitKey(0);
    return 0;
}