#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

Mat src, gray, dst;

Mat cannyEdges;
int cannyLowThreshold = 1;

Mat sobelGradx, sobelGrady, sobelAbsGradx, sobelAbsGrady;
int sobelKernelSize = 1;

Mat scharrGradx, scharrGrady, scharrAbsGradx, scharrAbsGrady;

static void applyScharr()
{

    // x
    Scharr(src, scharrGradx, CV_16S, 1, 0, 1.0, 1.0, BORDER_DEFAULT);
    convertScaleAbs(scharrGradx, scharrAbsGradx);

    // y
    Scharr(src, scharrGrady, CV_16S, 0, 1, 1.0, 1.0, BORDER_DEFAULT);
    convertScaleAbs(scharrGrady, scharrAbsGrady);

    addWeighted(scharrAbsGradx, 0.5, scharrAbsGrady, 0.5, 0, dst);
    imshow("Scharr", dst);
}

static void onCanny(int, void *)
{
    blur(gray, cannyEdges, Size(3, 3));
    Canny(cannyEdges, cannyEdges, cannyLowThreshold, cannyLowThreshold * 3, 3);
    dst = Scalar::all(0);
    src.copyTo(dst, cannyEdges);
    imshow("Canny", dst);
}

static void onSobel(int, void *)
{
    Sobel(src, sobelGradx, CV_16S, 1, 0, sobelKernelSize * 2 + 1, 1.0, 1.0, BORDER_DEFAULT);
    convertScaleAbs(sobelGradx, sobelAbsGradx);

    Sobel(src, sobelGrady, CV_16S, 0, 1, sobelKernelSize * 2 + 1, 1.0, 1.0, BORDER_DEFAULT);
    convertScaleAbs(sobelGrady, sobelAbsGrady);

    addWeighted(sobelAbsGradx, 0.5, sobelAbsGrady, 0.5, 0, dst);
    imshow("Sobel", dst);
}

static int edgeDetection(const std::string &imgname)
{
    src = imread(imgname);
    if (src.empty()) {
        return 0;
    }
    imshow("Origin", src);

    dst.create(src.size(), src.type());

    cvtColor(src, gray, COLOR_BGR2GRAY);

    namedWindow("Canny");
    namedWindow("Sobel");

    createTrackbar("Threshold", "Canny", &cannyLowThreshold, 120, &onCanny);
    createTrackbar("Size", "Sobel", &sobelKernelSize, 3, &onSobel);
    onCanny(0, 0);
    onSobel(0, 0);

    applyScharr();
    return 0;
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    edgeDetection(imgname);

    waitKey(0);
    return 0;
}