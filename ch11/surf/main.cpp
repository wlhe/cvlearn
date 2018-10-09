#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace cv::xfeatures2d;

static void surfKeyPoint(int argc, char **argv)
{
    std::string file1 = "../../images/1.jpg";
    std::string file2 = "../../images/2.jpg";
    if (argc == 3) {
        file1 = argv[1];
        file2 = argv[2];
    }
    Mat src1 = imread(file1);
    Mat src2 = imread(file2);
    if (src1.empty() || src2.empty()) {
        std::cout << " Image empty\n";
    }
    imshow("src1", src1);
    imshow("src2", src2);

    int minHessian = 400;
    cv::Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(minHessian);
    std::vector<KeyPoint> keypoint1, keypoint2;

    detector->detect(src1, keypoint1);
    detector->detect(src2, keypoint2);

    Mat img1, img2;
    drawKeypoints(src1, keypoint1, img1);
    drawKeypoints(src2, keypoint2, img2);

    imshow("img1", img1);
    imshow("img2", img2);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    surfKeyPoint(argc, argv);

    waitKey(0);
    return 0;
}