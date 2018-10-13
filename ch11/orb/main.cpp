#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace cv::xfeatures2d;

#define USE_SURF
static int orbDemo(int argc, char **argv)
{
    std::string file1 = "../../images/3.jpg";
    if (argc == 2) {
        file1 = argv[1];
    }
    Mat src = imread(file1);
    if (src.empty()) {
        std::cout << " Image empty\n";
    }
    imshow("Origin", src);

    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    std::vector<KeyPoint> keypoint1;
    Mat descriptor1;
    cv::Ptr<ORB> detector = ORB::create();
    cv::Ptr<ORB> extractor = ORB::create();

    detector->detect(gray, keypoint1);
    extractor->compute(gray, keypoint1, descriptor1);

    cv::flann::Index flannIndex(descriptor1, cv::flann::LshIndexParams(12, 20, 2),
                                cvflann::FLANN_DIST_HAMMING);

    VideoCapture cap(0);
    unsigned int frame = 0;
    while (char(waitKey(1)) != 'q') {
        int64 time0 = getTickCount();

        Mat capImg, capGray;
        cap >> capImg;
        if (capImg.empty()) {
            continue;
        }
        cvtColor(capImg, capGray, COLOR_BGR2GRAY);

        std::vector<KeyPoint> keypoint2;
        detector->detect(capGray, keypoint2);
        Mat descriptor2(keypoint2.size(), 2, CV_8UC1);
        extractor->compute(capGray, keypoint2, descriptor2);

        Mat matchIndex(descriptor2.rows, 2, CV_32SC1);
        Mat matchDistance(descriptor2.rows, 2, CV_32FC1);
        flannIndex.knnSearch(descriptor2, matchIndex, matchDistance, 2);

        std::vector<DMatch> goodMatches;
        for (int i = 0; i < matchDistance.rows; i++) {
            if (matchDistance.at<float>(i, 0) < 0.6 * matchDistance.at<float>(i, 1)) {
                DMatch dmatch(i, matchIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
                goodMatches.push_back(dmatch);
            }
        }

        Mat dstImg;
        drawMatches(capImg, keypoint2, src, keypoint1, goodMatches, dstImg, Scalar(0, 0, 255), Scalar(255, 0, 0),
                std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        double fps = (double)getTickFrequency() / (getTickCount() - time0);
        char text[32] = {0};
        snprintf(text, 32, "Fps: %.03f", fps);
        putText(dstImg, text, Point(30, 60), FONT_HERSHEY_COMPLEX, 2, Scalar(50, 200, 50), 2);
        imshow("Matches", dstImg);
    }
    return 0;
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    orbDemo(argc, argv);

    waitKey(0);
    return 0;
}