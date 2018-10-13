#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/calib3d.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace cv::xfeatures2d;

static void findObject(int argc, char **argv)
{
    std::string file1 = "../../images/4.jpg";
    std::string file2 = "../../images/5.jpg";
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
    std::vector<KeyPoint> kpObject, kpScene;

    detector->detect(src1, kpObject);
    detector->detect(src2, kpScene);

    cv::Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();

    Mat descripObject, descripScene;
    extractor->compute(src1, kpObject, descripObject);
    extractor->compute(src2, kpScene, descripScene);

    Ptr<FlannBasedMatcher> matcher = FlannBasedMatcher::create();
    std::vector<DMatch> matches;
    matcher->match(descripObject, descripScene, matches);

    double minDist = 100.0, maxDist = 0.0;
    for (int i = 0; i < descripObject.rows; i++) {
        double dist = matches[i].distance;
        if (dist < minDist) {minDist = dist;}
        if (dist > maxDist) {maxDist = dist;}
    }

    printf("minDsit %f, maxDist %f\n", minDist, maxDist);

    std::vector<DMatch> goodMatches;
    for (int i = 0; i < descripObject.rows; i++) {
        if (matches[i].distance < 3 * minDist) {
            goodMatches.push_back(matches[i]);
        }
    }

    Mat img;
    drawMatches(src1, kpObject, src2, kpScene, goodMatches, img, Scalar(0, 0, 255), Scalar(255, 0, 0),
                std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    std::vector<Point2f> obj, scene;

    for (int i = 0; i < goodMatches.size(); i++) {
        obj.push_back(kpObject[goodMatches[i].queryIdx].pt);
        scene.push_back(kpObject[goodMatches[i].trainIdx].pt);
    }

    Mat h = findHomography(obj, scene, CV_RANSAC);

    std::vector<Point2f> objCorners(4);
    objCorners[0] = Point(0, 0);
    objCorners[1] = Point(src1.cols, 0);
    objCorners[2] = Point(src1.cols, src1.rows);
    objCorners[3] = Point(0, src1.rows);

    std::vector<Point2f> sceneCorners(4);

    perspectiveTransform(objCorners, sceneCorners, h);

    line(img, sceneCorners[0] + Point2f(static_cast<float>(src1.cols), 0),
         sceneCorners[1] + Point2f(static_cast<float>(src1.cols), 0), Scalar(255, 0, 123), 4);
    line(img, sceneCorners[1] + Point2f(static_cast<float>(src1.cols), 0),
         sceneCorners[2] + Point2f(static_cast<float>(src1.cols), 0), Scalar(255, 0, 123), 4);
    line(img, sceneCorners[2] + Point2f(static_cast<float>(src1.cols), 0),
         sceneCorners[3] + Point2f(static_cast<float>(src1.cols), 0), Scalar(255, 0, 123), 4);
    line(img, sceneCorners[3] + Point2f(static_cast<float>(src1.cols), 0),
         sceneCorners[0] + Point2f(static_cast<float>(src1.cols), 0), Scalar(255, 0, 123), 4);

    imshow("matches", img);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    findObject(argc, argv);

    waitKey(0);
    return 0;
}