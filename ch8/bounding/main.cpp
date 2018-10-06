#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

static void sampleArea(const std::string &imgname)
{
    Mat image(600, 600, CV_8UC3);

    RNG &rng = theRNG();

    while (1) {
        int count = (unsigned)rng % 100 + 3;
        vector<Point> points;

        for (int i = 0; i < count; i++) {
            Point p;
            p.x = rng.uniform(image.cols / 4, image.cols * 3 / 4);
            p.y = rng.uniform(image.rows / 4, image.rows * 3 / 4);
            points.push_back(p);
        }

        image = Scalar::all(0);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        for (int i = 0; i < count; i++) {
            circle(image, points[i], 3, color, FILLED, LINE_AA);
        }
        // Rectangle
        RotatedRect box = minAreaRect(points);
        Point2f vertex[4];
        box.points(vertex);
        Scalar cl = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        for (int i = 0; i < 4; i++) {
            line(image, vertex[i], vertex[(i + 1) % 4], cl, 2, LINE_AA);
        }

        // Circle
        Point2f center;
        float radius = 0;
        minEnclosingCircle(points, center, radius);
        circle(image, center, radius, color);

        imshow("Result", image);

        char key = waitKey();
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }
    }
}
////
Mat g_srcImage;
Mat g_grayImage;
int g_nThresh = 50;
int g_maxThresh = 255;
RNG g_rng(12345);
static void onContoursChange(int, void *)
{
    Mat threshOut;
    threshold(g_grayImage, threshOut, g_nThresh, 255, THRESH_BINARY);

    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(threshOut, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
#if 0
    vector< vector<Point> > contoursPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());

    for (int i = 0; i < contours.size(); i++) {
        approxPolyDP(contours[i], contoursPoly[i], 3, true);
        boundRect[i] = boundingRect(contoursPoly[i]);
        minEnclosingCircle(contoursPoly[i], center[i], radius[i]);
    }

    Mat drawing = Mat::zeros(threshOut.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
        drawContours(drawing, contoursPoly, i, color);
        rectangle(drawing, boundRect[i], color, 2);
        circle(drawing, center[i], radius[i], color);
    }
#else
    Mat drawing = Mat::zeros(threshOut.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        vector<Point> contoursPoly;
        approxPolyDP(contours[i], contoursPoly, 3, true);
        Rect boundRect = boundingRect(contours[i]);
        Point2f center; float radius = 0.0f;
        minEnclosingCircle(contours[i], center, radius);

        Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
        vector< vector<Point> > drawContoursPoly = {contoursPoly};
        drawContours(drawing, drawContoursPoly, 0, color);
        rectangle(drawing, boundRect, color, 1);
        circle(drawing, center, radius, color, 2);
    }
#endif
    imshow("Result", drawing);
}

static void convexAreaDemo(const std::string &imgname)
{
    g_srcImage = imread(imgname);
    if (g_srcImage.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", g_srcImage);

    cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
    blur(g_grayImage, g_grayImage, Size(3, 3));

    namedWindow("Origin");

    createTrackbar("Threshold", "Origin", &g_nThresh, 255, onContoursChange);
    onContoursChange(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleArea(imgname);

    convexAreaDemo(imgname);


    waitKey(0);
    return 0;
}