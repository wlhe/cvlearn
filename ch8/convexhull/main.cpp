#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

static void sampleConvexHull(const std::string &imgname)
{
    Mat image(600, 600, CV_8UC3);

    RNG &rng = theRNG();

    while (1) {
        int count = (unsigned)rng % 100 + 3;
        vector<Point> points;

        for (int i = 0; i < count; i++) {
            Point p;
            p.x = rng.uniform(0, image.cols);
            p.y = rng.uniform(0, image.rows);
            points.push_back(p);
        }

        image = Scalar::all(0);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        for (int i = 0; i < count; i++) {
            circle(image, points[i], 3, color, FILLED, LINE_AA);
        }
        // vector<int> hull;
        // convexHull(points, hull, true);
        // Point p0 = points[hull[hull.size() - 1]]; // last point
        // Scalar cl = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        // for (int i = 0; i < hull.size(); i++) {
        //     Point p1 = points[hull[i]];
            // line(image, p0, p1, cl, 2, LINE_AA);
        //     p0 = p1;
        // }
        vector< vector<Point> > hull(1);
        convexHull(points, hull[0], true);
        drawContours(image, hull, 0, color, 2);

        imshow("Result", image);

        char key = waitKey();
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }
    }
}


Mat g_srcImage;
Mat g_grayImage;
int g_nThresh = 50;
int g_maxThresh = 255;
RNG g_rng(12345);
Mat srcImage_copy;
Mat g_thresholdImage_output;
vector<vector<Point> > g_vContours;
vector<Vec4i> g_vHierarchy;
static void onThreshold(int, void *)
{
    threshold(g_grayImage, g_thresholdImage_output, g_nThresh, 255, THRESH_BINARY);

    findContours(g_thresholdImage_output, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector< vector<Point> > hull(g_vContours.size());
    for (int i = 0; i < g_vContours.size(); i++) {
        convexHull(g_vContours[i], hull[i], false);
    }
    Mat drawing = Mat::zeros(g_thresholdImage_output.size(), CV_8UC3);
    for (int i = 0; i < g_vContours.size(); i++) {
        Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
        drawContours(drawing, g_vContours, i, color);
        drawContours(drawing, hull, i, color);
    }
    imshow("Convex", drawing);
}
static void convexHullDemo(const std::string &imgname)
{
    g_srcImage = imread(imgname);
    if (g_srcImage.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", g_srcImage);
    srcImage_copy = g_srcImage.clone();

    cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
    blur(g_grayImage, g_grayImage, Size(3, 3));

    namedWindow("Convex");

    createTrackbar("Threshold", "Origin", &g_nThresh, 255, onThreshold);
    onThreshold(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/1.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleConvexHull(imgname);

    convexHullDemo(imgname);

    waitKey(0);
    return 0;
}