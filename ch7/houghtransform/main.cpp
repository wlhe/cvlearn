#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void sampleHough(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat mid, dst;

    cvtColor(src, mid, COLOR_BGR2GRAY);
    blur(mid, mid, Size(3, 3));
    Canny(mid, mid, 50, 200, 3);
    imshow("mid", mid);

    src.copyTo(dst);

#if 0 // HoughLines
    std::vector<Vec2f> lines;
    HoughLines(mid, lines, 1, CV_PI/180, 150, 0, 0);

    for (int i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0], theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = rho * a, y0 = rho * b;
        Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
        Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
        line(dst, pt1, pt2, Scalar(50, 150, 50), 1, LINE_AA);
    }
#endif
    // HoughLinesP
    std::vector<Vec4i> lines2;
    HoughLinesP(mid, lines2, 1, CV_PI / 180, 80, 100, 10);

    for (int i = 0; i < lines2.size(); ++i) {
        Point pt1(lines2[i][0], lines2[i][1]);
        Point pt2(lines2[i][2], lines2[i][3]);
        line(dst, pt1, pt2, Scalar(20, 20, 200), 1, LINE_AA);
    }

    imshow("Result", dst);
}

static void sampleHoughCircle(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat mid, dst;

    cvtColor(src, mid, COLOR_BGR2GRAY);
    GaussianBlur(mid, mid, Size(9, 9), 2, 2);
    imshow("mid", mid);

    src.copyTo(dst);

    std::vector<Vec3f> circles;
    HoughCircles(mid, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);

    for (int i = 0; i < circles.size(); ++i) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        circle(dst, center, 5, Scalar(0, 100, 200), -1, 8, 0);

        circle(dst, center, radius, Scalar(50, 100, 200), 2, 8, 0);
    }

    imshow("Result", dst);
}

// demo
Mat g_srcImage, g_dstImage, g_midImage;
std::vector<Vec4i> g_lines;
int g_nthreshold = 100;
static void on_HoughLines(int, void *)
{
    Mat dstImage = g_dstImage.clone();
    Mat midImage = g_midImage.clone();

    std::vector<Vec4i> mylines;
    HoughLinesP(midImage, mylines, 1, CV_PI / 180, g_nthreshold + 1, 50, 10);

    for (size_t i = 0; i < mylines.size(); i++) {
        Vec4i l = mylines[i];
        line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23, 180, 55), 1, LINE_AA);
    }
    imshow("Result", dstImage);
}
static void houghDemo(const std::string &imgname)
{
    g_srcImage = imread(imgname);

    imshow("Origin", g_srcImage);

    namedWindow("Result", 1);
    createTrackbar("Threshold", "Result", &g_nthreshold, 200, on_HoughLines);

    Canny(g_srcImage, g_midImage, 50, 200, 3);
    cvtColor(g_midImage, g_dstImage, COLOR_GRAY2BGR);

    on_HoughLines(g_nthreshold, 0);
    HoughLinesP(g_midImage, g_lines, 1, CV_PI / 180, 80, 50, 10);

    imshow("Result", g_dstImage);
}


int main(int argc, char **argv)
{
    std::string imgname = "../../images/6.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // sampleHough(imgname);

    // sampleHoughCircle(imgname);

    houghDemo(imgname);

    waitKey(0);
    return 0;
}