#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void dilateErode(const std::string &imgname)
{
    Mat img = imread(imgname);

    imshow("Origin", img);

    Mat out;
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

    dilate(img, out, element);

    imshow("Dilate", out);

    Mat out2;

    erode(img, out2, element);

    imshow("Erode", out2);
}

//
static Mat g_srcImage, g_dstImage;
int g_nTrackBarNumber = 0;
int g_nStructElementSize = 3;

static void process()
{
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1),
                                        Point(g_nStructElementSize, g_nStructElementSize));
    if (g_nTrackBarNumber == 0) {
        erode(g_srcImage, g_dstImage, element);
    } else {
        dilate(g_srcImage, g_dstImage, element);
    }
    imshow("Result", g_dstImage);
}

static void on_trackBarNumChange(int, void *)
{
    process();
}

static void on_elementSizeChange(int, void *)
{
    process();
}

static void imageDilateErode(const cv::String &imgname)
{
    g_srcImage = imread(imgname);
    if (!g_srcImage.data) {
        std::cout << "Image read error!" << std::endl;
        return;
    }
    imshow("Origin", g_srcImage);

    cv::String wndName = "Result";
    namedWindow(wndName);

    Mat element = getStructuringElement(MORPH_RECT, Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1),
                                        Point(g_nStructElementSize, g_nStructElementSize));
    erode(g_srcImage, g_dstImage, element);
    imshow(wndName, g_dstImage);

    createTrackbar("Erode/Dilate", wndName, &g_nTrackBarNumber, 1, &on_trackBarNumChange);
    createTrackbar("Kernel Size", wndName, &g_nStructElementSize, 21, &on_elementSizeChange);

    while (char(waitKey(50)) != 'q') {
    }
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/5.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // dilateErode(imgname);
    imageDilateErode(imgname);

    waitKey(0);
    return 0;
}