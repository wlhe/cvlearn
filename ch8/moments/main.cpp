#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat g_srcImage;
Mat g_grayImage;
int g_nThresh = 50;
int g_maxThresh = 255;
Mat g_cannyImage_output;
vector<vector<Point> > g_vContours;
vector<Vec4i> g_vHierarchy;
static void onThreshold(int, void *)
{
    Canny(g_grayImage, g_cannyImage_output, g_nThresh, g_nThresh * 2, 3);

    findContours(g_cannyImage_output, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector <Moments> mu(g_vContours.size());
    vector <Point2f> mc(g_vContours.size());
    for (int i = 0; i < g_vContours.size(); i++) {
        mu[i] = moments(g_vContours[i], false);
        mc[i] = Point2f(float(mu[i].m10/mu[i].m00), float(mu[i].m01/mu[i].m00));
    }

    Mat drawing = Mat::zeros(g_cannyImage_output.size(), CV_8UC3);
    for (int i = 0; i < g_vContours.size(); i++) {
        Scalar color = Scalar(theRNG().uniform(0, 255), theRNG().uniform(0, 255), theRNG().uniform(0, 255));
        drawContours(drawing, g_vContours, i, color);
        circle(drawing, mc[i], 4, color, -1, 8, 0);
    }
    imshow("Area", drawing);

    for (int i = 0; i < g_vContours.size(); i++) {
        printf("m00 mu[%d] = %.2f\n"
               "OpenCV s = %.2f, l %.2f\n\n",
               i, mu[i].m00,
               contourArea(g_vContours[i]),
               arcLength(g_vContours[i], true));
    }
}
static void momentsDemo(const std::string &imgname)
{
    g_srcImage = imread(imgname);
    if (g_srcImage.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", g_srcImage);

    cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
    blur(g_grayImage, g_grayImage, Size(3, 3));

    createTrackbar("Threshold", "Origin", &g_nThresh, 255, onThreshold);
    onThreshold(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/5.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    momentsDemo(imgname);

    waitKey(0);
    return 0;
}