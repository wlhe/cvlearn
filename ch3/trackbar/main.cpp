#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

static Mat img1 = imread("images/1.jpg");
static Mat img2 = imread("images/2.jpg");
static Mat imgdst;
static int value = 70;
static int maxValue = 100;
static std::string winName = "Linear mixed";

static void trackBarCallback(int pos, void *userdata)
{
    int max = *(static_cast<int *>(userdata));
    double alpha = (double)pos / max;
    double beta = 1.0 - alpha;
    addWeighted(img1, alpha, img2, beta, 0.0, imgdst);
    imshow(winName, imgdst);
}

static void trackBar()
{

    img1 = imread("images/1.jpg");
    img2 = imread("images/2.jpg");

    if (!img1.data || !img2.data) {
        fprintf(stderr, "Read image error\n");
        return;
    }

    char name[32] = { 0 };
    snprintf(name, 32, "Alpha %d", maxValue);
    
    namedWindow(winName);
    auto i = 1;

    createTrackbar(name, winName, &value, maxValue, trackBarCallback, &maxValue);

    trackBarCallback(value, &maxValue);
}

int main()
{
    trackBar();

    waitKey(0);
    return 0;
}