#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

//
static Mat g_srcImage, g_dstImage;
int g_nElementShape = MORPH_RECT;
int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackHatNum = 0;

static void on_openClose(int, void *)
{
    int offset = g_nOpenCloseNum - g_nMaxIterationNum;
    int absoffset = abs(offset);
    Mat element = cv::getStructuringElement(g_nElementShape, Size(absoffset * 2 + 1, absoffset * 2 + 1),
                                            Point(absoffset, absoffset));
    if (offset > 0) {
        cv::morphologyEx(g_srcImage, g_dstImage, MORPH_OPEN, element);
    } else {
        cv::morphologyEx(g_srcImage, g_dstImage, MORPH_CLOSE, element);
    }
    imshow("Open/Close", g_dstImage);
}

static void on_erodeDilate(int, void *)
{
        int offset = g_nErodeDilateNum - g_nMaxIterationNum;
    int absoffset = abs(offset);
    Mat element = cv::getStructuringElement(g_nElementShape, Size(absoffset * 2 + 1, absoffset * 2 + 1),
                                            Point(absoffset, absoffset));
    if (offset > 0) {
        // cv::erode(g_srcImage, g_dstImage, element);
        cv::morphologyEx(g_srcImage, g_dstImage, MORPH_ERODE, element);
    } else {
        // cv::dilate(g_srcImage, g_dstImage, element);
        cv::morphologyEx(g_srcImage, g_dstImage, MORPH_DILATE, element);
    }
    imshow("Erode/Dilate", g_dstImage);
}

static void on_topBlackHat(int, void *)
{
    int offset = g_nTopBlackHatNum - g_nMaxIterationNum;
    int absoffset = abs(offset);
    Mat element = cv::getStructuringElement(g_nElementShape, Size(absoffset * 2 + 1, absoffset * 2 + 1),
                                            Point(absoffset, absoffset));
    if (offset > 0) {
        cv::morphologyEx(g_srcImage, g_dstImage, MORPH_TOPHAT, element);
    } else {
        cv::morphologyEx(g_srcImage, g_dstImage, MORPH_BLACKHAT, element);
    }
    imshow("TopHat/BlackHat", g_dstImage);
}

static void morphology(const std::string &imgname)
{
    g_srcImage = imread(imgname);
    if (!g_srcImage.data) {
        std::cout << "Image read error!" << std::endl;
        return;
    }
    imshow("Origin", g_srcImage);

    namedWindow("Open/Close");
    namedWindow("Erode/Dilate");
    namedWindow("TopHat/BlackHat");

    g_nOpenCloseNum = 9;
    g_nErodeDilateNum = 9;
    g_nTopBlackHatNum = 2;

    createTrackbar("IterationNum", "Open/Close", &g_nOpenCloseNum, g_nMaxIterationNum * 2 + 1, &on_openClose);
    createTrackbar("IterationNum", "Erode/Dilate", &g_nErodeDilateNum, g_nMaxIterationNum * 2 + 1, &on_erodeDilate);
    createTrackbar("IterationNum", "TopHat/BlackHat", &g_nTopBlackHatNum, g_nMaxIterationNum * 2 + 1, &on_topBlackHat);

    while (1) {
        on_openClose(g_nOpenCloseNum, 0);
        on_erodeDilate(g_nErodeDilateNum, 0);
        on_topBlackHat(g_nTopBlackHatNum, 0);

        char c = char(waitKey(0));

        if (c == 'q' || c == 27) {
            break;
        } else if (c == '1') {
            g_nElementShape = MORPH_RECT;
        } else if (c == '2') {
            g_nElementShape = MORPH_ELLIPSE;
        } else if (c == '3') {
            g_nElementShape = MORPH_CROSS;
        } else if (c == ' ') {
            g_nElementShape = (g_nElementShape + 1) % 3;
        }
    }
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/8.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    morphology(imgname);

    waitKey(0);
    return 0;
}