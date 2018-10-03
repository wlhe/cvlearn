#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;


static void help()
{
    std::cout << "Threshold Type: \n"
                 "\t0 - THRESH_BINARY\n"
                 "\t1 - THRESH_BINARY_INV\n"
                 "\t2 - THRESH_TRUNC\n"
                 "\t3 - THRESH_TOZERO\n"
                 "\t4 - THRESH_TOZERO_INV\n"
                 "\th - print this help\n"
              << std::endl;
}

int g_nThresholdValue = 100;
int g_nThresholdType = 3;
Mat srcImage;
Mat grayImage;
Mat dstImage;

static void onThreshold(int, void *)
{
    threshold(grayImage, dstImage, g_nThresholdValue, 255, g_nThresholdType);
    imshow("Result", dstImage);
}

static int thresholddemo(int argc, char **argv)
{
    std::string filename = "../../images/12.jpg";
    if (argc == 2) {
        filename = argv[1];
    }
    srcImage = imread(filename);
    if (srcImage.empty()) {
        std::cout << "Image empty" << std::endl;
        return 0;
    }
    help();

    imshow("Origin", srcImage);
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    imshow("Gray", grayImage);

    namedWindow("Result");

    createTrackbar("Threshold Type", "Result", &g_nThresholdType, 4, onThreshold);
    createTrackbar("Threshold Value", "Result", &g_nThresholdValue, 255, onThreshold);

    onThreshold(0, 0);

    for (;;) {
        char c = char(waitKey(0));
        if (c == 27) { // ESC
            break;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/12.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    thresholddemo(argc, argv);

    waitKey(0);
    return 0;
}