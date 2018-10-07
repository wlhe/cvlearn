#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

int main(int argc, char **argv)
{
    Mat src, src1, src2;
    Mat hsv, hsv1, hsv2;
    src = imread("../1.jpg");
    src1 = imread("../2.jpg");
    src2 = imread("../3.jpg");

    imshow("src", src);
    imshow("src1", src1);
    imshow("src2", src2);

    cvtColor(src, hsv, COLOR_BGR2HSV);
    cvtColor(src1, hsv1, COLOR_BGR2HSV);
    cvtColor(src2, hsv2, COLOR_BGR2HSV);

    Mat hsvHalfdown = hsv(Range(hsv.rows / 2, hsv.rows - 1), Range(0, hsv.cols - 1));

    int hbins = 50;
    int sbins = 60;
    int histSize[] = {hbins, sbins};
    float hranges[] = {0, 180};
    float sranges[] = {0, 256};
    const float *ranges[] = {hranges, sranges};
    int channels[] = {0, 1};
    MatND hist, hist1, hist2, histHalfdown;

    calcHist(&hsv, 1, channels, Mat(), hist, 2, histSize, ranges);
    normalize(hist, hist, 0, 1, NORM_MINMAX);

    calcHist(&hsv1, 1, channels, Mat(), hist1, 2, histSize, ranges);
    normalize(hist1, hist1, 0, 1, NORM_MINMAX);

    calcHist(&hsv2, 1, channels, Mat(), hist2, 2, histSize, ranges);
    normalize(hist2, hist2, 0, 1, NORM_MINMAX);

    calcHist(&hsvHalfdown, 1, channels, Mat(), histHalfdown, 2, histSize, ranges);
    normalize(histHalfdown, histHalfdown, 0, 1, NORM_MINMAX);

    for(int i = 0; i < 4; i ++) {
        int method = i;
        double comp = compareHist(hist, hist, method);
        double compHalfdown = compareHist(hist, histHalfdown, method);
        double comp1 = compareHist(hist, hist1, method);
        double comp2 = compareHist(hist, hist2, method);
        printf("method %d, comp %f, half %f, comp1 %f, comp2 %f\n",
               i, comp, compHalfdown, comp1, comp2);
    }


    waitKey(0);
    return 0;
}