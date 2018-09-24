#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <cstdio>

using namespace cv;

static void dftEample()
{
    Mat srcImage = imread("../../images/2.jpg", IMREAD_GRAYSCALE);
    if (!srcImage.data) {
        std::cout << "Image read error" << std::endl;
        return;
    }
    // printf("Origin [%d, %d]\n", srcImage.rows, srcImage.cols);
    imshow("Origin", srcImage);

    int m = getOptimalDFTSize(srcImage.rows);
    int n = getOptimalDFTSize(srcImage.cols);

    Mat padded;
    copyMakeBorder(srcImage, padded, 0, m - srcImage.rows, 0, n - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

    // printf("Padded [%d, %d]\n", padded.rows, padded.cols);
    // imshow("Padded", padded);

    Mat planes[] = {
        Mat1f(padded), Mat::zeros(padded.size(), CV_32F)
    };
    Mat complexI;
    merge(planes, 2, complexI);

    dft(complexI, complexI);

    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magnitudeImage = planes[0];

    magnitudeImage += Scalar::all(1);
    log(magnitudeImage, magnitudeImage);
    // (a & -2), means (even number & 0xFFFFFFFE), get a even number not greater than a
    magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));
    int cx = magnitudeImage.cols / 2, cy = magnitudeImage.rows / 2;
    Mat q0(magnitudeImage, Rect(0, 0, cx, cy));
    Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));
    Mat q2(magnitudeImage, Rect(0, cy, cx, cy));
    Mat q3(magnitudeImage, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magnitudeImage, magnitudeImage, NORM_MINMAX);

    imshow("Frequency magnitude", magnitudeImage);
}

int main()
{
    dftEample();
    waitKey(0);
    return 0;
}