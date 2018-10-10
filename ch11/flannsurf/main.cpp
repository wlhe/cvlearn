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
static int flannSurfDemo(int argc, char **argv)
{
    std::string file1 = "../../images/3.jpg";
    if (argc == 2) {
        file1 = argv[1];
    }
    Mat trainImg = imread(file1);
    if (trainImg.empty()) {
        std::cout << " Image empty\n";
    }
    imshow("Origin", trainImg);

    Mat trainGray;
    cvtColor(trainImg, trainGray, COLOR_BGR2GRAY);

    std::vector<KeyPoint> keypoint1;
    Mat descriptor1;
#ifdef USE_SURF
        cv::Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(80);
        cv::Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();
#else
        cv::Ptr<SiftFeatureDetector> detector = SiftFeatureDetector::create(80);
        cv::Ptr<SiftDescriptorExtractor> extractor = SiftDescriptorExtractor::create();
#endif
    detector->detect(trainGray, keypoint1);
    extractor->compute(trainGray, keypoint1, descriptor1);

    Ptr<FlannBasedMatcher> matcher = FlannBasedMatcher::create();
    std::vector<Mat> trainDescCollection(1, descriptor1);
    matcher->add(trainDescCollection);
    matcher->train();

    VideoCapture cap(0);
    unsigned int frame = 0;
    while (char(waitKey(1)) != 'q') {
        int64 time0 = getTickCount();

        Mat testImg, testGray;
        cap >> testImg;
        if (testImg.empty()) {
            continue;
        }
        cvtColor(testImg, testGray, COLOR_BGR2GRAY);

        std::vector<KeyPoint> keypoint2;
        Mat descriptor2;

        detector->detect(testGray, keypoint2);
        extractor->compute(testGray, keypoint2, descriptor2);

        std::vector< std::vector<DMatch> > matches;
        matcher->knnMatch(descriptor2, matches, 2);

        std::vector<DMatch> goodMatches;
        for (int i = 0; i < matches.size(); i++) {
            if (matches[i][0].distance < 0.6 * matches[i][1].distance) {
                goodMatches.push_back(matches[i][0]);
            }
        }

        Mat dstImg;
        drawMatches(testImg, keypoint2, trainImg, keypoint1, goodMatches, dstImg, Scalar(0, 0, 255), Scalar(255, 0, 0),
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

    flannSurfDemo(argc, argv);

    waitKey(0);
    return 0;
}