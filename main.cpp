#include "opencv2/core/core.hpp"        // Mat!
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"  // imread, imWindow and so
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"
#include "detection.h"
#include <iostream>
#include <string>

using namespace cv;
using namespace cv::ml;
using namespace std;

size_t  GOOD_NUM_OF_POINTS = 5;

int main(int argc, char* argv[])
{

    Detection* detector = new Detection(1, 1);
    detector->train();

    vector <Point2d> points;

    VideoCapture cap(1); // open the video camera no. 0
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    while (1){
        Mat image_raw, // = imread("/home/lysuhin/Coding/OpenCV/examples/test_image_4.jpg");
            image_show;

        #if 1
        bool bSuccess = cap.read(image_raw);
        if (!bSuccess){
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        #endif // 0

        image_raw.copyTo(image_show);

        detector->detect(image_show, points);
        cout << "points number = " << points.size() << endl;

        imshow("1", image_raw);
        imshow("2", image_show);

        if (waitKey(1) == 27){
            break;
        }

        if (points.size() > GOOD_NUM_OF_POINTS){
            cout << "Looks like a good picture. Use it? (y/n)" << endl;
            char c = waitKey(3000);
            if (c == 'y'){
                cout << "Okay! Processing the picture..." << endl;
                destroyAllWindows();
                image_raw.copyTo(image_show);
                detector->detect(image_show, points, true, true, true);
                imshow("3", image_show);
                waitKey();
                break;
            }
        }
        points.clear();
    }
    return 0;
}
