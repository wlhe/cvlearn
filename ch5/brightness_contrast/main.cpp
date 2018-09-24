#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <cstdio>

using namespace cv;

static int g_nContrastValue, g_nBrightnessValue;
static Mat g_srcImage, g_dstImage;

static const std::string resultWindow = "Result";

static void onContrastAndBrightness(int pos, void *userdata)
{
    for (int i = 0; i < g_srcImage.rows; ++i) {
        for (int j = 0; j < g_srcImage.cols; ++j) {
            for (int k = 0; k < g_srcImage.channels(); ++k) {
                g_dstImage.at<Vec3b>(i, j)[k] = saturate_cast<uchar>(
                     g_nContrastValue * 0.01 * g_srcImage.at<Vec3b>(i, j)[k] + g_nBrightnessValue);
            }
        }
    }
    printf("Brightness: %d, Contrast: %d\n", g_nBrightnessValue, g_nContrastValue);
    imshow(resultWindow, g_dstImage);
}

static void contrastBrightness()
{
    g_srcImage = imread("../../images/1.jpg");
    if (!g_srcImage.data) {
        std::cout << "Image read error" << std::endl;
        return;
    }

    g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

    g_nContrastValue = 80;
    g_nBrightnessValue = 80;

    namedWindow(resultWindow);

    createTrackbar("Britghtness", resultWindow, &g_nBrightnessValue, 200, onContrastAndBrightness);
    createTrackbar("Contrast", resultWindow, &g_nContrastValue, 300, onContrastAndBrightness);

    onContrastAndBrightness(g_nBrightnessValue, 0);
    onContrastAndBrightness(g_nContrastValue, 0);

    imshow("Origin", g_srcImage);

    while (waitKey(0) == 'q') {

    }
}

int main()
{
    contrastBrightness();
    waitKey(0);
    return 0;
}