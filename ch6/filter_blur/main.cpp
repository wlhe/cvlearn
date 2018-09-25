#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <vector>

using namespace cv;
///
static void imageBluring(const std::string &imgname)
{
    Mat img = imread(imgname);

    imshow("Origin", img);

    Mat out;

    boxFilter(img, out, -1, Size(15, 15));
    imshow("boxFilter", out);

    blur(img, out, Size(7, 7));
    imshow("blur", out);

    GaussianBlur(img, out, Size(3, 3), 0.3, 0.2);
    imshow("GaussianBlur", out);

    medianBlur(img, out, 7);
    imshow("medianBlur", out);

    bilateralFilter(img, out, 25, 25 * 2, 25 / 2);
    imshow("bilateralFilter", out);
}
//
//
//
static Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5;
static int g_nBoxFilterValue = 6;
static int g_nMeanBlurValue = 10;
static int g_nGaussianBlurValue = 6;
static int g_nMedianBlurValue = 10;
static int g_nBilateralFilterValue = 10;

static void onBoxFilter(int, void *)
{
    boxFilter(g_srcImage, g_dstImage1, -1, Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
    imshow("BoxFilter", g_dstImage1);
}
static void onMeanBlur(int, void *)
{
    blur(g_srcImage, g_dstImage2, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1));
    imshow("MeanBlur", g_dstImage2);
}
static void onGaussianBlur(int, void *)
{
    GaussianBlur(g_srcImage, g_dstImage3, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);
    imshow("GaussianBlur", g_dstImage3);
}
static void onMedianBlur(int, void *)
{
    medianBlur(g_srcImage, g_dstImage4, g_nMedianBlurValue * 2 + 1);
    imshow("MedianBlur", g_dstImage4);
}
static void onBilateralFilter(int, void *)
{
    bilateralFilter(g_srcImage, g_dstImage5, g_nBilateralFilterValue,
                    g_nBilateralFilterValue * 2, g_nBilateralFilterValue / 2);
    imshow("BilateralFilter", g_dstImage5);
}

static void imageBlur(const std::string &imgname)
{
    g_srcImage = imread(imgname);
    if (!g_srcImage.data) {
        std::cout << "Image read error!" << std::endl;
        return;
    }

    imshow("Origin", g_srcImage);

    g_dstImage1 = g_srcImage.clone();
    g_dstImage2 = g_srcImage.clone();
    g_dstImage3 = g_srcImage.clone();

    namedWindow("BoxFilter");
    createTrackbar("Kvalue", "BoxFilter", &g_nBoxFilterValue, 40, onBoxFilter);
    onBoxFilter(g_nBoxFilterValue, 0);

    namedWindow("MeanBlur");
    createTrackbar("Kvalue", "MeanBlur", &g_nMeanBlurValue, 40, onMeanBlur);
    onMeanBlur(g_nMeanBlurValue, 0);

    namedWindow("GaussianBlur");
    createTrackbar("Kvalue", "GaussianBlur", &g_nGaussianBlurValue, 40, onGaussianBlur);
    onGaussianBlur(g_nGaussianBlurValue, 0);

    namedWindow("MedianBlur");
    createTrackbar("Kvalue", "MedianBlur", &g_nMedianBlurValue, 40, onMedianBlur);
    onMedianBlur(g_nMedianBlurValue, 0);

    namedWindow("BilateralFilter");
    createTrackbar("Kvalue", "BilateralFilter", &g_nBilateralFilterValue, 40, onBilateralFilter);
    onBilateralFilter(g_nBilateralFilterValue, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // imageBluring(imgname);
    imageBlur(imgname);

    waitKey(0);
    return 0;
}