#include "camera.h"

void mat2Buff(cv::Mat* im,unsigned char buff[]){
    uchar* p;
    for (int i = 0; i < im->rows; ++i) {
        p = im->ptr<uchar>(i);
        for (int j = 0; j < im->cols; ++j) buff[i * im -> cols + j] = p[j];
    }
}
void buff2Mat(cv::Mat* im, unsigned char buff[]){
    uchar* p;
    for (int i = 0; i < im->rows; ++i) {
        p = im->ptr<uchar>(i);
        for (int j = 0; j < im->cols; ++j) p[j] = buff[i * im -> cols + j];
    }
}

int Camera::getFrame(){
    cap.read(frame);
    
    if (frame.empty()) {
        perror("ERROR! blank frame grabbed\n");
        return -1;
    }
    cv::cvtColor(frame,bwframe, cv::COLOR_RGB2GRAY);
    return 0;
}

int Camera::getFrameAsBuffer(unsigned char buff[]){
    getFrame();
    mat2Buff(&bwframe, buff);
    return 0;
}

int Camera::open(){    
    cap.open(deviceID + apiID);
    if (!cap.isOpened()) {
        perror("ERROR! Unable to open camera\n");
        return -1;
    }
    return 0;
}