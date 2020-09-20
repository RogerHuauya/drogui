#ifndef ELP_H
#define ELP_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/stat.h> 
#include <string>
#include "utils.h"

struct elp{
    private:
        cv::Mat left_img, right_img, deep_img;
        std::vector<cv::Point2f> left_corner_pts,right_corner_pts;
        utils::FPS fps;
        int width,height;
        cv::VideoCapture cap;

        int font = cv::FONT_HERSHEY_SIMPLEX;
        std::string FOLDER ="results";
        int CHECKERBOARD[2]{6,9};
    public:
        elp(int id, int w,int h);
        void create_folders();
        void getCalibrationData();
        void calibrate();
        std::vector<cv::Point2f>  getCornersPoints(cv::Mat *frame);
        bool create_folder(std::string name);
        void getFrames();
        void processDeep();
        void create_capture(int id, int w,int h);
        void destroyAllWindows();
        bool waitKey();
};

#endif