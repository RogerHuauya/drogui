#ifndef CAMERA_H
#define CAMERA_H
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

struct Camera{
    int deviceID, apiID;
    cv::VideoCapture cap;
    cv::Mat frame;
    cv::Mat bwframe;
    int getFrame();
    int getFrameAsBuffer(unsigned char buff[]);
    int open();
    
    Camera(int _deviceID){
        deviceID = _deviceID;
        apiID = cv::CAP_ANY;
    }
    Camera(){};
};

void buff2Mat(cv::Mat* im, unsigned char buff[]);
void mat2Buff(cv::Mat* im, unsigned char buff[]);

#endif