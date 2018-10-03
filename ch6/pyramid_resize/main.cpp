#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;



static void imageReize(const std::string &imgname)
{
    Mat srcImage = imread(imgname);

    imshow("Origin", srcImage);

    Mat tmpImage, dstImage1, dstImage2;
    tmpImage = srcImage;

    cv::resize(tmpImage, dstImage1, Size(tmpImage.cols / 2, tmpImage.rows / 2), 0.0, 0.0, 3);
    cv::resize(tmpImage, dstImage2, tmpImage.size() * 2, 0.0, 0.0, 3);

    imshow("Result 1", dstImage1);
    imshow("Result 2", dstImage2);
}


static void pyrUpDown(const std::string &imgname)
{
    Mat srcImage = imread(imgname);

    imshow("Origin", srcImage);

    Mat tmpImage, dstImage1, dstImage2;
    tmpImage = srcImage;

    pyrUp(tmpImage, dstImage1, tmpImage.size() * 2);
    imshow("pyrUp", dstImage1);

    pyrDown(tmpImage, dstImage2, tmpImage.size() / 2);
    imshow("pyrDown", dstImage2);
}


static void help()
{
    std::cout << "Hot keys: \n"
            "\tq - quit the program\n"
            "\tw - resize up\n"
            "\ts - resize down\n"
            "\ta - pyrDown\n"
            "\td - pyrUp\n"
            "\th - print this help\n" << std::endl;
}

static int pyramiddemo(int argc, char **argv)
{
    std::string filename = "../../images/10.jpg";
    if (argc == 2) {
        filename = argv[1];
    }
    Mat srcImage = imread(filename);
    if (srcImage.empty()) {
        std::cout << "Image empty" << std::endl;
        return 0;
    }
    help();

    namedWindow("Pyramid");
    imshow("Pyramid", srcImage);

    Mat tmpImage, dstImage;
    tmpImage = srcImage;
    dstImage = srcImage;

    for (;;) {
        char c = (char)waitKey(10);
        if (c == 'q') {
            break;
        }
        switch (c) {
        case 'w':
            resize(tmpImage, dstImage, tmpImage.size() * 2);
            break;
        case 's':
            resize(tmpImage, dstImage, tmpImage.size() / 2);
            break;
        case 'a':
            pyrDown(tmpImage, dstImage, tmpImage.size() / 2);
            break;
        case 'd':
            pyrUp(tmpImage, dstImage, tmpImage.size() * 2);
            break;
        default:
            break;
        }
        imshow("Pyramid", dstImage);
        tmpImage = dstImage;
    }
    return 0;
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/11.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // imageReize(imgname);
    // pyrUpDown(imgname);

    pyramiddemo(argc, argv);

    waitKey(0);
    return 0;
}