#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

int main(int argc, char **argv)
{
    std::string imgname = "../../images/9.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    Mat src = imread(imgname, IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat dst;
    equalizeHist(src, dst);

    imshow("Result", dst);

    waitKey(0);
    return 0;
}