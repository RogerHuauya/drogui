#ifndef ELP_H
#define ELP_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/stat.h> 
#include <string>
#include "utils.h"

struct elp{
    private:
        utils::FPS fps;
        cv::Mat left_img, right_img, deep_img;
        std::vector<cv::Point2f> left_corner_pts,right_corner_pts;
        std::vector<std::vector<cv::Point3f> > left_objpoints,right_objpoints;
        std::vector<std::vector<cv::Point2f> > left_imgpoints,right_imgpoints;
        std::vector<cv::Point3f> aux_objp;
        int width,height;
        cv::VideoCapture cap;
        int font = cv::FONT_HERSHEY_SIMPLEX;
        cv::Mat left_cameraMatrix,left_distCoeffs,left_R,left_T;
        cv::Mat right_cameraMatrix,right_distCoeffs,right_R,right_T;

        std::string FOLDER ="results";
        int CHECKERBOARD[2]{6,9};
        float square_size = 0.02423;
        cv::TermCriteria TERMINATION_CRITERIA = cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER, 30, 0.01);
        cv::TermCriteria calib_criteria = cv::TermCriteria(cv::TermCriteria::EPS +cv::TermCriteria::MAX_ITER, 10, 1e-6);
        int calibration_flags = cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC |  cv::fisheye::CALIB_FIX_SKEW;

        cv::Mat rotationMatrix;
        cv::Mat translationVector;
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
        void init_chess();
};

#endif