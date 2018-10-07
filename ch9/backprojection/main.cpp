#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static Mat src, hsv, hue;
static int bins = 30;
static void onBinChanged(int, void *)
{
    int histSize[] = {MAX(bins, 2)};
    float hranges[] = {0, 180};
    const float *ranges[] = {hranges};
    MatND hist;
    int channels[] = {0};

    calcHist(&hsv, 1, channels, Mat(), hist, 1, histSize, ranges);
    normalize(hist, hist, 0, 255, NORM_MINMAX);

    MatND backproj;
    calcBackProject(&hue, 1, channels, hist, backproj, ranges, 1, true);

    imshow("Back projection", backproj);

    int w = 400, h = 400;
    Mat histImg = Mat::zeros(w, h, CV_8UC3);
    int binw = cvRound(w / histSize[0]);

    for (int i = 0; i < bins; i++) {
        rectangle(histImg, Point(i * binw, h - 1),
                  Point((i + 1) * binw, h - cvRound(hist.at<float>(i) * h / 255.0)),
                  Scalar(100, 100, 200));
    }
    imshow("Hist", histImg);
}

static void backProjectionDemo(const std::string &imgname)
{
    src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    cvtColor(src, hsv, COLOR_BGR2HSV);

    hue.create(hsv.size(), hsv.type());
    int ch[] = {0, 0};
    mixChannels(&hsv, 1, &hue, 1, ch, 1);

    namedWindow("Origin");
    createTrackbar("Bin Value", "Origin", &bins, 180, onBinChanged);
    onBinChanged(0, 0);
}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/4.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    backProjectionDemo(imgname);

    waitKey(0);
    return 0;
}