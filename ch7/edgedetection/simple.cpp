#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void sampleCanny(const std::string &imgname)
{
    Mat src = imread(imgname);
    imshow("Origin", src);

    Mat src1 = src.clone();
    Mat dst, gray, edge;

    dst.create(src1.size(), src1.type());

    cvtColor(src1, gray, cv::COLOR_BGR2GRAY);
    blur(gray, edge, Size(3, 3));
    Canny(edge, edge, 3, 9, 3);

    imshow("Edge", edge);

    dst = Scalar::all(0);
    src1.copyTo(dst, edge);
    imshow("Result", dst);
}

static void sampleSobel(const std::string &imgname)
{
    Mat src = imread(imgname);
    imshow("Origin", src);

    Mat gradx, grady, absGradx, absGrady;

    // x
    Sobel(src, gradx, CV_16S, 1, 0, 3, 1.0, 1.0, BORDER_DEFAULT);
    // Scharr(src, gradx, CV_16S, 1, 0, 1.0, 1.0, BORDER_DEFAULT);
    convertScaleAbs(gradx, absGradx);
    imshow("absGradx", absGradx);

    // y
    Sobel(src, grady, CV_16S, 0, 1, 3, 1.0, 1.0, BORDER_DEFAULT);
    // Scharr(src, grady, CV_16S, 0, 1, 1.0, 1.0, BORDER_DEFAULT);
    convertScaleAbs(grady, absGrady);
    imshow("absGrady", absGrady);

    Mat dst;
    addWeighted(absGradx, 0.5, absGrady, 0.5, 0, dst);
    imshow("Result", dst);
}

static void sampleLaplacian(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        return;
    }
    imshow("Origin", src);

    Mat gray, dst, absDst;

    GaussianBlur(src, src, Size(3, 3), 0.0);
    cvtColor(src, gray, COLOR_BGR2GRAY);

    Laplacian(gray, dst, CV_16S, 3, 1.0, 0.0, BORDER_DEFAULT);
    convertScaleAbs(dst, absDst);

    imshow("Result", absDst);
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleCanny(imgname);

    sampleSobel(imgname);

    // sampleLaplacian(imgname);

    waitKey(0);
    return 0;
}