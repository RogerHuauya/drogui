#include "elp.h"
#include <iostream>

void elp::getCalibrationData(){

}

void elp::calibrate(){
    std::cout<<" calibration xxd"<<std::endl;
}

void elp::getFrames(){
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) return;
    left_img = frame(cv::Range(0,height), cv::Range(0,width/2)); //240x320
    right_img = frame(cv::Range(0,height), cv::Range(width/2,width)); //240x320
}

void elp::processDeep(){

}

void elp::create_capture(int id,int w, int h){
    cap.open(id);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,w);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,h); 
    width  = cap.get(3);
    height = cap.get(4);
    std::cout<<"height: "<<height<<" width: "<<width<<std::endl;        
};

void elp::destroyAllWindows(){
    cv::destroyAllWindows();
};