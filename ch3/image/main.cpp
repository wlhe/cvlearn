#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

using namespace cv;
using namespace std;

static void createAlphaMat(Mat &mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b &bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX; // Blue
            bgra[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
        }
    }
}

static void imgWrite()
{
    Mat mat(480, 640, CV_8UC4);
    createAlphaMat(mat);

    vector<int> compression_params = { IMWRITE_PNG_COMPRESSION, 9 };

    try {
        imwrite("images/alpha.png", mat, compression_params);
        imshow("Alpah", mat);
    }
    catch (cv::Exception &ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
    }
    fprintf(stdout, "Saved PNG file with alpha data.\n");
}

static void imgShowWrite()
{
    Mat girl = imread("images/girl.jpg");
    namedWindow("girl");
    imshow("girl", girl);

    Mat image = imread("images/dota.jpg");
    Mat logo = imread("images/dota_logo.jpg");

    imshow("Origin", image);
    imshow("Logo", logo);

    Mat imageROI = image(Rect(800, 350, logo.cols, logo.rows));

    addWeighted(imageROI, 0.9, logo, 0.1, 0.0, imageROI);

    imshow("Origin + Logo", image);

    imwrite("images/dota_add_logo.jpg", image);
}

int main()
{
    //imgWrite();

    imgShowWrite();

    waitKey(0);
    return 0;
}