#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "Usage: ./main pic_name" << endl;
        return -1;
    }
    Mat image;
    char *file = argv[1];
    image = imread(file);

    if (!image.data) {
        cout << "Could not read picture" << endl;
        return -1;
    }

    imshow("Window", image);
    waitKey(0);

    return 0;
}