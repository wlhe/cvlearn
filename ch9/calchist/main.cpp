#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

static void hueSaturationHist(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    int hbins = 30;
    int sbins = 32;
    int histSize[] = {hbins, sbins};
    float hranges[] = {0, 180};
    float sranges[] = {0, 255};
    const float *ranges[] = {hranges, sranges};
    MatND hist;
    int channels[] = {0, 1};

    calcHist(&hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);

    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 20;
    Mat histImg = Mat::zeros(sbins * scale, hbins * scale, CV_8UC3);

    for (int h = 0; h < hbins; h++) {
        for (int s = 0; s < sbins; s++) {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal * 255 / maxVal);
            rectangle(histImg, Point(h * scale, s * scale),
                      Point((h + 1) * scale - 1, (s + 1) * scale - 1),
                      Scalar::all(intensity), cv::FILLED);
        }
    }

    imshow("H-S Histogram", histImg);
}

static void sample1Dhist(const std::string &imgname)
{
    Mat src = imread(imgname, IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    MatND hist;
    int dims = 1;
    float hranges[] = {0, 255};
    const float *ranges[] = {hranges};
    int channels[] = {0};
    int size = 256;

    calcHist(&src, 1, channels, Mat(), hist, dims, &size, ranges);

    double minVal = 0, maxVal = 0;
    minMaxLoc(hist, &minVal, &maxVal, 0, 0);

    int scale = 1;
    Mat histImg = Mat::zeros(size * scale, size * scale, CV_8UC3);

    int hpt = size * 0.9;
    for (int i = 0; i < size; i++) {
        float binVal = hist.at<float>(i);
        int realValue = saturate_cast<int>(binVal * hpt / maxVal);
        rectangle(histImg, Point(i * scale, size * scale - 1),
                  Point((i + 1) * scale - 1, (size - realValue) * scale - 1),
                  Scalar::all(255));
    }
    imshow("1D Hist", histImg);
}

static void sampleRGBHist(const std::string &imgname)
{
    Mat src = imread(imgname);
    if (src.empty()) {
        std::cout << imgname << " Image empty\n";
    }
    imshow("Origin", src);

    int bins = 256;
    int histSize[] = {bins};
    float range[] = {0, 256};
    const float *ranges[] = {range};
    MatND histRed, histGreen, histBlue;

    int channelsBlue[] = {0};
    calcHist(&src, 1, channelsBlue, Mat(), histBlue, 1, histSize, ranges);

    int channelsGreen[] = {1};
    calcHist(&src, 1, channelsGreen, Mat(), histGreen, 1, histSize, ranges);

    int channelsRed[] = {2};
    calcHist(&src, 1, channelsRed, Mat(), histRed, 1, histSize, ranges);

    double maxRed, maxGreen, maxBlue;
    minMaxLoc(histBlue, 0, &maxBlue, 0, 0);
    minMaxLoc(histGreen, 0, &maxGreen, 0, 0);
    minMaxLoc(histRed, 0, &maxRed, 0, 0);

    int scale = 1;
    Mat histImg = Mat::zeros(bins * scale, bins * 3, CV_8UC3);

    for (int i = 0; i < bins; i++) {
        float binRed = histRed.at<float>(i);
        float binGreen = histGreen.at<float>(i);
        float binBlue = histBlue.at<float>(i);
        int intensityRed = cvRound(binRed * bins / maxRed);
        int intensityGreen = cvRound(binGreen * bins / maxGreen);
        int intensityBlue = cvRound(binBlue * bins / maxBlue);

        rectangle(histImg, Point(i * scale, bins - 1),
                  Point((i + 1) * scale - 1, bins - intensityBlue - 1),
                  Scalar(255, 0, 0));
        rectangle(histImg, Point((i + bins) * scale, bins - 1),
                  Point((i + bins + 1) * scale - 1, bins - intensityGreen - 1),
                  Scalar(0, 255, 0));
        rectangle(histImg, Point((i + 2 * bins) * scale, bins - 1),
                  Point((i + 2 * bins + 1) * scale - 1, bins - intensityRed - 1),
                  Scalar(0, 0, 255));
    }

    imshow("RGB Hist", histImg);

}

int main(int argc, char **argv)
{
    std::string imgname = "../../images/3.jpg";
    if (argc == 2) {
        imgname = argv[1];
    }

    // hueSaturationHist(imgname);

    // sample1Dhist(imgname);

    sampleRGBHist(imgname);

    waitKey(0);
    return 0;
}