#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo/photo.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat srcImage1, inpaintMask;
Point prevPt(-1, -1);

static void onMouse(int event, int x, int y, int flags, void *)
{
    if (x < 0 || x >= srcImage1.cols || y < 0 || y >= srcImage1.rows)
        return;

    if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON)) {
        prevPt = Point(-1, -1);
    } else if (event == EVENT_LBUTTONDOWN) {
        prevPt = Point(x, y);
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
        Point pt(x, y);
        if (prevPt.x < 0)
            prevPt = pt;
        line(inpaintMask, prevPt, pt, Scalar::all(255), 5, 8, 0);
        line(srcImage1, prevPt, pt, Scalar::all(255), 5, 8, 0);
        prevPt = pt;
        imshow("Origin", srcImage1);
        // imshow("inpaintMask", inpaintMask);
    }
}
static void inpaintDemo(const std::string &imgname)
{
    srcImage1 = imread(imgname);
    if (srcImage1.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", srcImage1);

    Mat srcImage = srcImage1.clone();
    inpaintMask = Mat::zeros(srcImage1.size(), CV_8UC1);

    setMouseCallback("Origin", onMouse);

    while (1) {
        int c = waitKey(0);

        if ((char)c == 27)
            break;

        if ((char)c == '2') {
            inpaintMask = Scalar::all(0);
            srcImage.copyTo(srcImage1);
            imshow("Origin", srcImage1);
        }

        if ((char)c == '1' || (char)c == ' ') {
            Mat inpaintedImage;
            inpaint(srcImage1, inpaintMask, inpaintedImage, 3, INPAINT_TELEA);
            imshow("Inpaint", inpaintedImage);
        }
    }
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/7.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    inpaintDemo(imgname);

    waitKey(0);
    return 0;
}