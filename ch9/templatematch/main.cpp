#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static Mat src, templ, result;
int matchMethod = 0;
int maxTrackBarNum = 5;

static void onMatching(int, void *)
{
    Mat src1 = src.clone();
    result.create(src.rows - templ.rows + 1, src.cols - templ.cols + 1, CV_32FC1);

    matchTemplate(src, templ, result, matchMethod);
    normalize(result, result, 0, 1, NORM_MINMAX);

    double minVal = 0, maxVal = 0;
    Point minLoc, maxLoc;
    Point matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }

    rectangle(src1, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
              Scalar(0, 0, 255), 2);

    rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
              Scalar(0, 0, 255), 2);

    imshow("src", src1);
    imshow("Result", result);
}

static void templageMatchDemo()
{
    src = imread("../1.jpg");
    templ = imread("../2.jpg");
    imshow("src", src);
    imshow("temp", templ);

    namedWindow("Result");
    createTrackbar("Method", "src", &matchMethod, maxTrackBarNum, onMatching);
    onMatching(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/4.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    templageMatchDemo();

    waitKey(0);
    return 0;
}