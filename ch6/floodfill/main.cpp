#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

static void floodfill(const std::string &imgname)
{
    Mat img = imread(imgname);

    imshow("Origin", img);

    Rect ccomp;
    floodFill(img, Point(50, 300), Scalar(200, 100, 5), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

    imshow("FloodFill", img);
}

// FloodFill Example
// opencv-3.4.0/samples/cpp/ffilldemo.cpp
static void help()
{
    cout << "\nThis program demonstrated the floodFill() function\n"
            "Call:\n"
            "./ffilldemo [image_name -- Default: ../data/fruits.jpg]\n" << endl;

    cout << "Hot keys: \n"
            "\tESC - quit the program\n"
            "\tc - switch color/grayscale mode\n"
            "\tm - switch mask mode\n"
            "\tr - restore the original image\n"
            "\ts - use null-range floodfill\n"
            "\tf - use gradient floodfill with fixed(absolute) range\n"
            "\tg - use gradient floodfill with floating(relative) range\n"
            "\t4 - use 4-connectivity mode\n"
            "\t8 - use 8-connectivity mode\n"
            "\th - print this help\n" << endl;
}

//
Mat image0, image, gray, mask;
int ffillMode = 1;
int loDiff = 20, upDiff = 20;
int connectivity = 4;
bool isColor = true;
bool useMask = false;
int newMaskVal = 255;

static void onMouse( int event, int x, int y, int, void* )
{
    if (event != cv::EVENT_LBUTTONDOWN) {
        return;
    }

    Point seed = Point(x, y);
    int lo = ffillMode == 0 ? 0 : loDiff;
    int up = ffillMode == 0 ? 0 : upDiff;
    int flags = connectivity + (newMaskVal << 8) + (ffillMode == 1 ? FLOODFILL_FIXED_RANGE : 0);
    int b = (unsigned)theRNG() & 255;
    int g = (unsigned)theRNG() & 255;
    int r = (unsigned)theRNG() & 255;

    Rect ccomp;
    Scalar newval = isColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);
    Mat dst = isColor ? image : gray;
    int area;

    if (useMask) {
        threshold(mask, mask, 1, 128, THRESH_BINARY);
        area = floodFill(dst, mask, seed, newval, &ccomp, Scalar(lo, lo, lo), Scalar(up, up, up), flags);
        imshow("mask", mask);
    } else {
        area = floodFill(dst, mask, seed, newval, &ccomp, Scalar(lo, lo, lo), Scalar(up, up, up), flags);
    }

    imshow("image", dst);
    cout << area << " pixels were repainted\n";
}

static int ffilldemo(int argc, char **argv)
{
    string filename = "../../images/10.jpg";
    if (argc == 2) {
        filename = argv[1];
    }
    image0 = imread(filename);
    if (image0.empty()) {
        cout << "Image empty" << endl;
        return 0;
    }
    help();

    image0.copyTo(image);
    cvtColor(image0, gray, COLOR_BGR2GRAY);
    mask.create(image0.rows + 2, image0.cols + 2, CV_8UC1);

    namedWindow("image");
    createTrackbar("lo_diff", "image", &loDiff, 255, 0);
    createTrackbar("up_diff", "image", &upDiff, 255, 0);

    setMouseCallback("image", &onMouse, 0);

    for (;;) {
        imshow("image", isColor ? image : gray);

        char c = char(waitKey(0));
        if (c == 27)  { // ESC
            cout << "Existing..." << endl;
            break;
        }

        switch (c) {
        case 'c':
        case '1':
            if (isColor) {
                cout << "Grayscale mode is set\n";
                cvtColor(image0, gray, COLOR_BGR2GRAY);
                mask = Scalar::all(0);
                isColor = false;
            } else {
                cout << "Color mode is set\n";
                image0.copyTo(image);
                isColor = true;
            }
            break;
        case 'm':
        case '2':
            if (useMask) {
                destroyWindow("mask");
                useMask = false;
            } else {
                namedWindow("mask");
                mask = Scalar::all(0);
                imshow("mask", mask);
                useMask = true;
            }
            break;
        case 'r':
        case '3':
            cout << "Original image is restored\n";
            image0.copyTo(image);
            cvtColor(image0, gray, COLOR_BGR2GRAY);
            mask = Scalar::all(0);
            break;
        case 's':
            cout << "Simple floodfill is set\n";
            ffillMode = 0;
            break;
        case 'f':
        case '5':
            cout << "Fixed Range floodfill mode is set\n";
            ffillMode = 1;
            break;
        case 'g':
        case '6':
            cout << "Gradient (floating range) floodfill mode is set\n";
            ffillMode = 2;
            break;
        case '4':
        case '7':
            cout << "4-connectivity mode is set\n";
            connectivity = 4;
            break;
        case '8':
            cout << "8-connectivity mode is set\n";
            connectivity = 8;
            break;
        case 'h':
            help();
            break;
        default:
            break;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/9.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // floodfill(imgname);
    ffilldemo(argc, argv);

    waitKey(0);
    return 0;
}