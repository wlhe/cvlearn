#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static bool multiChannelBlenging()
{
    Mat srcImage = imread("../../images/dota_jugg.jpg");
    Mat logo = imread("../../images/dota_logo.jpg", IMREAD_GRAYSCALE);
    Mat logo2 = imread("../../images/dota_logo.jpg");
    if (!srcImage.data || !logo.data) {
        std::cerr << "Read image error!!" << std::endl;
    }
    Mat dstImage = srcImage.clone();

    std::vector<Mat> channels;
    split(srcImage, channels);
    double alpha = 0.5, beta = 0.5;
    Mat roi;

    Mat bChannel = channels[0];
    roi = bChannel(Rect(330, 20, logo.cols, logo.rows));

    addWeighted(roi, alpha, logo, beta, 0, roi);

    Mat gChannel = channels[1];
    roi = gChannel(Rect(550, 20, logo.cols, logo.rows));

    addWeighted(roi, alpha, logo, beta, 0, roi);

    Mat rChannel = channels[2];
    roi = rChannel(Rect(330, 240, logo.cols, logo.rows));

    addWeighted(roi, alpha, logo, beta, 0, roi);

    merge(channels, dstImage);

    roi = dstImage(Rect(550, 240, logo2.cols, logo2.rows));
    addWeighted(roi, 0.8, logo2, 0.2, 0, roi);

    imshow("Blending Logo Red Channel", dstImage);

    // imshow("logo", logo);

    return true;
}

int main()
{
    if (multiChannelBlenging()) {
        std::cout << "Run ok" << std::endl;
    }
    waitKey(0);
    return 0;
}