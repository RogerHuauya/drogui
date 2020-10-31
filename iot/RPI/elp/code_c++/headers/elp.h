#ifndef ELP_H
#define ELP_H

#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <sys/stat.h> 
#include <string>
#include "utils.h"

struct elp{
    struct subcamera{
        cv:: Mat img;
        std::vector<cv::Point2f> corners_pts;
        std::vector<std::vector<cv::Point3f> > objpoints;
        std::vector<std::vector<cv::Point2f> > imgpoints;
        cv::Mat cameraMatrix,distCoeffs;
        std::vector< cv::Mat > Rvecs,Tvecs;
        std::string name;
        cv::Mat mapx, mapy;
        cv::Rect roi;
    };
    private:
        int id,w,h;
        cv::Mat R1, R2, P1, P2, Q;
        utils::FPS fps;
        cv::Mat deep_img;
        subcamera left,right;
        cv::VideoCapture cap;
        int width,height;
        std::vector<cv::Point3f> aux_objp;
        int font = cv::FONT_HERSHEY_SIMPLEX;
        std::string FOLDER ="results";
        int CHECKERBOARD[2]{6,9};
        float square_size = 0.02423;
        int flag = 0;
        cv::TermCriteria TERMINATION_CRITERIA = cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER, 30, 0.01);
        cv::TermCriteria calib_criteria = cv::TermCriteria(cv::TermCriteria::EPS +cv::TermCriteria::MAX_ITER, 10, 1e-6);
        int calibration_flags = cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC |  cv::fisheye::CALIB_FIX_SKEW;
        cv::Mat rotationMatrix;
        cv::Mat translationVector;
    public:
        elp(int id_, int w_,int h_);
        cv::Ptr<cv::StereoBM> stereoMatcher = cv::StereoBM::create(128,7);
        void see_rectify();
        void create_folders();
        void getCalibrationData();
        void calibrate();
        double errorcalibration_camera(struct subcamera& cam);
        void undistort(struct subcamera& cam);
        void calibrate_camera(struct subcamera& cam,std::string name);
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