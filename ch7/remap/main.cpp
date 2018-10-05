#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void sampleRemap(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat mapx, mapy, dst;
    mapx.create(src.size(), CV_32FC1);
    mapy.create(src.size(), CV_32FC1);
    dst.create(src.size(), src.type());

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            mapx.at<float>(i, j) = float(src.cols - j);
            mapy.at<float>(i, j) = float(src.rows - i);
        }
    }

    remap(src, dst, mapx, mapy, INTER_LINEAR);

    imshow("Result", dst);
}

Mat g_srcImage, g_dstImage;
Mat g_map_x, g_map_y;

static void updateMap(int key)
{
    for (int j = 0; j < g_srcImage.rows; j++) {
        for (int i = 0; i < g_srcImage.cols; i++) {
            switch (key) {
            case '1':
                if (i > g_srcImage.cols * 0.25 && i < g_srcImage.cols * 0.75 && j > g_srcImage.rows * 0.25 &&
                    j < g_srcImage.rows * 0.75) {
                    g_map_x.at<float>(j, i) = static_cast<float>(2 * (i - g_srcImage.cols * 0.25) + 0.5);
                    g_map_y.at<float>(j, i) = static_cast<float>(2 * (j - g_srcImage.rows * 0.25) + 0.5);
                } else {
                    g_map_x.at<float>(j, i) = 0;
                    g_map_y.at<float>(j, i) = 0;
                }
                break;
            case '2':
                g_map_x.at<float>(j, i) = static_cast<float>(i);
                g_map_y.at<float>(j, i) = static_cast<float>(g_srcImage.rows - j);
                break;
            case '3':
                g_map_x.at<float>(j, i) = static_cast<float>(g_srcImage.cols - i);
                g_map_y.at<float>(j, i) = static_cast<float>(j);
                break;
            case '4':
                g_map_x.at<float>(j, i) = static_cast<float>(g_srcImage.cols - i);
                g_map_y.at<float>(j, i) = static_cast<float>(g_srcImage.rows - j);
                break;
            }
        }
    }
}
static void remapDemo(const std::string &imgname)
{
    g_srcImage = imread(imgname);
    if (g_srcImage.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", g_srcImage);

    g_dstImage.create(g_srcImage.size(), g_srcImage.type());
    g_map_x.create(g_srcImage.size(), CV_32FC1);
    g_map_y.create(g_srcImage.size(), CV_32FC1);

    namedWindow("Result", WINDOW_AUTOSIZE);
    imshow("Result", g_srcImage);

    while (1) {
        int key = waitKey(0);

        if ((key & 255) == 27) {
            std::cout << "Exit...\n";
            break;
        }

        updateMap(key);

        remap(g_srcImage, g_dstImage, g_map_x, g_map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

        imshow("Result", g_dstImage);
    }
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/9.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleRemap(imgname);

    remapDemo(imgname);

    waitKey(0);
    return 0;
}