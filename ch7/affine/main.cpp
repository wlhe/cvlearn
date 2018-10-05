#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;


static void affineDemo(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Point2f srcTriangle[3];
    Point2f dstTriangle[3];

    Mat rotMat(2, 3, CV_32FC1);
    Mat warpMat(2, 3, CV_32FC1);

    Mat dst, dstRotate;
    srcTriangle[0] = Point2f(0, 0);
    srcTriangle[1] = Point2f(float(src.cols - 1), 0);
    srcTriangle[2] = Point2f(0, float(src.rows - 1));

    dstTriangle[0] = Point2f(float(src.cols * 0.0), float(src.rows * 0.33));
    dstTriangle[1] = Point2f(float(src.cols * 0.65), float(src.rows * 0.35));
    dstTriangle[2] = Point2f(float(src.cols * 0.15), float(src.rows * 0.6));

    warpMat = getAffineTransform(srcTriangle, dstTriangle);

    warpAffine(src, dst, warpMat, src.size());

    Point center = Point(dst.cols / 2, dst.rows / 2);
    double angle = -50.0;
    double scale = 0.6;

    rotMat = getRotationMatrix2D(center, angle, scale);
    warpAffine(dst, dstRotate, rotMat, dst.size());

    imshow("warp", dst);
    imshow("rot", dstRotate);
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/9.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    affineDemo(imgname);

    waitKey(0);
    return 0;
}