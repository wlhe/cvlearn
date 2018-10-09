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

static void surfMatch(int argc, char **argv)
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

    cv::Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();

    Mat descriptor1, descriptor2;
    extractor->compute(src1, keypoint1, descriptor1);
    extractor->compute(src2, keypoint2, descriptor2);

    Ptr<BFMatcher> matcher = BFMatcher::create();
    std::vector<DMatch> matches;
    matcher->match(descriptor1, descriptor2, matches);

    Mat img;
    drawMatches(src1, keypoint1, src2, keypoint2, matches, img, Scalar(0, 0, 255), Scalar(255, 0, 0));

    imshow("matches", img);
}

static void flannMatch(int argc, char **argv)
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
    cv::Ptr<SURF> detector = SURF::create(minHessian);
    std::vector<KeyPoint> keypoint1, keypoint2;

    detector->detect(src1, keypoint1);
    detector->detect(src2, keypoint2);

    cv::Ptr<SURF> extractor = SURF::create();

    Mat descriptor1, descriptor2;
    extractor->compute(src1, keypoint1, descriptor1);
    extractor->compute(src2, keypoint2, descriptor2);

    Ptr<FlannBasedMatcher> matcher = FlannBasedMatcher::create();
    std::vector<DMatch> matches;
    matcher->match(descriptor1, descriptor2, matches);

    double minDist = 100.0, maxDist = 0.0;
    for (int i = 0; i < descriptor1.rows; i++) {
        double dist = matches[i].distance;
        if (dist < minDist) {minDist = dist;}
        if (dist > maxDist) {maxDist = dist;}
    }

    printf("minDsit %f, maxDist %f\n", minDist, maxDist);

    std::vector<DMatch> goodMatches;
    for (int i = 0; i < descriptor1.rows; i++) {
        if (matches[i].distance < 2 * minDist) {
            goodMatches.push_back(matches[i]);
        }
    }

    Mat img;
    drawMatches(src1, keypoint1, src2, keypoint2, goodMatches, img, Scalar(0, 0, 255), Scalar(255, 0, 0),
                std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    for (int i = 0; i < goodMatches.size(); i++) {
        printf("good matches[%d]: keypoint1 %d, keypoint2 %d, distance %f\n",
               i, goodMatches[i].queryIdx, goodMatches[i].trainIdx, goodMatches[i].distance);
    }
    imshow("matches", img);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // surfKeyPoint(argc, argv);

    // surfMatch(argc, argv);

    flannMatch(argc, argv);

    waitKey(0);
    return 0;
}