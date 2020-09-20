#include "elp.h"

std::vector<cv::Point2f> elp::getCornersPoints(cv::Mat *frame){
    cv::Mat gray;
    std::vector<cv::Point2f> corners;
    cv::cvtColor(*frame,gray,cv::COLOR_BGR2GRAY);
    bool success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),
    corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
    if (success){
        cv::TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);
        cv::cornerSubPix(gray,corners,cv::Size(11,11), cv::Size(-1,-1),criteria);
        cv::drawChessboardCorners(*frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),corners, success);
    }
    return corners;
};

elp::elp(int id, int w,int h){
    create_capture(id,w,h);
};
void elp::create_folders(){
    int size = FOLDER.size(),c=1;
    while (!(create_folder(FOLDER))){
        FOLDER = FOLDER.substr(0,size)+std::to_string(c);
        c++;
    }     
    create_folder(FOLDER+"/videos/");
    create_folder(FOLDER+"/images/");
    create_folder(FOLDER+"/images/left");
    create_folder(FOLDER+"/images/right");
};
bool elp::waitKey(){
    char k=(char) cv::waitKey(25);
    if (k == 'q' || k == 'Q') return true;
    return false;
};
void elp::getCalibrationData(){
    create_folders();
    while(1){
        fps.start();
        getFrames();
        cv::Scalar color = cv::Scalar(0,0,255);
        std::vector<cv::Point2f> left_cor = getCornersPoints(&left_img);
        std::vector<cv::Point2f> right_cor = getCornersPoints(&right_img);
        if (left_cor.size()==54 && right_cor.size()==54) color = cv::Scalar(255,0,0);
        cv::putText(left_img,fps.get(),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::putText(right_img,fps.get(),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::imshow( "camera1", left_img);
        cv::imshow( "camera2", right_img);
        if (waitKey()) break;
    };
    cap.release();
    destroyAllWindows();
}

bool elp::create_folder(std::string name){
	if (mkdir(name.c_str(), 0777) == -1){
        std::cout<<"Folder "<<name<<" exits"<<std::endl;
        return false;
    }
	else std::cout << "Directory created"<<std::endl;
    return true;
}
void elp::calibrate(){
    std::cout<<" calibration xxd"<<std::endl;
}

void elp::getFrames(){
    cv::Mat frame;
    cap >> frame;
    cv::flip(frame,frame,1);
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