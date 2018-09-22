#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


static std::string winName = "Mouse";
static Rect g_rect;
static bool g_bDrawing;
static RNG g_rng(12345);

void drawRectangle(cv::Mat &img, const cv::Rect &box)
{
    cv::rectangle(img, box, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));//Ëæ»úÑÕÉ«
}

void onMouseHandle(int event, int x, int y, int flags, void *param)
{
    Mat &image = *(static_cast<Mat *>(param));
    switch (event) {
    case EVENT_LBUTTONDOWN:
        g_bDrawing = true;
        g_rect = Rect(x, y, 0, 0);
        break;
    case EVENT_MOUSEMOVE:
        if (g_bDrawing) {
            g_rect.width = x - g_rect.x;
            g_rect.height = y - g_rect.y;
        }
        break;
    case EVENT_LBUTTONUP:
        g_bDrawing = false;
        if (g_rect.width < 0) {
            g_rect.x += g_rect.width;
            g_rect.width *= -1;
        }
        if (g_rect.height < 0) {
            g_rect.y += g_rect.height;
            g_rect.height *= -1;
        }
        drawRectangle(image, g_rect);
        break;
    default:
        break;
    }
}



int main()
{
    g_rect = Rect(-1, -1, 0, 0);
    Mat srcImage(600, 800, CV_8UC3), tempImage;
    srcImage.copyTo(tempImage);
    srcImage = Scalar::all(0);

    namedWindow(winName);
    setMouseCallback(winName, onMouseHandle, &srcImage);

    for (;;) {
        srcImage.copyTo(tempImage);
        if (g_bDrawing) {
            drawRectangle(tempImage, g_rect);
        }
        imshow(winName, tempImage);
        if (waitKey(10) == 27) {
            break;
        }
    }

    waitKey(0);
    return 0;
}