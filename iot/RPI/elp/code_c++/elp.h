#ifndef ELP_H
#define ELP_H

#include "opencv2/opencv.hpp"

struct elp{
    
    cv::Mat left_img, right_img, deep_img;
    int width,height;
    cv::VideoCapture cap;
    void getCalibrationData();
    void calibrate();
    void getFrames();
    void processDeep();
    void create_capture(int id, int w,int h);
    void destroyAllWindows();
};

#endif