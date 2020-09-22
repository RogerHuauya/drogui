#include "elp.h"


elp::elp(int id, int w,int h){
    create_capture(id,w,h);
    init_chess();
};
void elp::getCalibrationData(){
    create_folders();
    int c = 0;
    while(1){
        fps.start();
        getFrames();
        cv::Scalar color = cv::Scalar(0,0,255);
        std::vector<cv::Point2f> left_cor = getCornersPoints(&left_img);
        std::vector<cv::Point2f> right_cor = getCornersPoints(&right_img);
        
        if (left_cor.size()==54 && right_cor.size()==54){
            color = cv::Scalar(255,0,0);
            left_objpoints.push_back(aux_objp);
            left_imgpoints.push_back(left_cor);
            right_objpoints.push_back(aux_objp);
            right_imgpoints.push_back(right_cor);
            c++;
        }
        cv::putText(left_img,fps.get()+" :"+std::to_string(c),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::putText(right_img,fps.get()+" :"+std::to_string(c),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::imshow( "camera1", left_img);
        cv::imshow( "camera2", right_img);
        if (waitKey()) break;
    };
    cap.release();
    destroyAllWindows();
};
bool elp::create_folder(std::string name){
	if (mkdir(name.c_str(), 0777) == -1){
        std::cout<<"Folder "<<name<<" exits"<<std::endl;
        return false;
    }
	else std::cout << "Directory created"<<std::endl;
    return true;
};
void elp::calibrate(){
    cv::Vec3f pt(0, 0, 0);
    cv::Size DIM = cv::Size(left_img.rows,left_img.cols);
    //cv::calibrateCamera(left_objpoints,left_imgpoints, cv::Size(left_img.rows,left_img.cols),left_cameraMatrix, left_distCoeffs, left_R, left_T);
    std::cout<<"Starting left calibration .."<<std::endl;
    double rms = cv::fisheye::calibrate(left_objpoints, left_imgpoints,DIM,left_cameraMatrix, left_distCoeffs, left_R, left_T, calibration_flags, calib_criteria);
    cv::Mat map1, map2;
    cv::fisheye::initUndistortRectifyMap(left_cameraMatrix,left_distCoeffs,pt,left_cameraMatrix, DIM, CV_16SC2, map1, map2);
    cv::FileStorage fs1(FOLDER + "/calibration_camera_left.yml", cv::FileStorage::WRITE);
     if (fs1.isOpened()){
        fs1 << "map1" << map1 << "map2" << map2 << "objpoints" << left_objpoints << "imgpoints" <<
              left_imgpoints << "camera_matrix" << left_cameraMatrix << "distortion_coeff" << left_distCoeffs;
    }
    std::cout<<"Finished left calibration."<<std::endl;

    std::cout<<"Starting right calibration .."<<std::endl;
    cv::Mat right_map1, right_map2;

    //cv::calibrateCamera(right_objpoints,right_imgpoints, cv::Size(right_img.rows,right_img.cols),right_cameraMatrix, right_distCoeffs, right_R, right_T);
    rms = cv::fisheye::calibrate(right_objpoints, right_imgpoints,DIM,right_cameraMatrix, right_distCoeffs, right_R, right_T, calibration_flags, calib_criteria);
    cv::fisheye::initUndistortRectifyMap(right_cameraMatrix,right_distCoeffs,pt,right_cameraMatrix, DIM, CV_16SC2, right_map1, right_map2);
    cv::FileStorage fs2(FOLDER + "/calibration_camera_right.yml", cv::FileStorage::WRITE);
     if (fs2.isOpened()){
        fs2 << "map1" << right_map1 << "map2" << right_map2 << "objpoints" << right_objpoints << "imgpoints" <<
              right_imgpoints << "camera_matrix" << right_cameraMatrix << "distortion_coeff" << right_distCoeffs;
    }
    std::cout<<"Finished right calibration."<<std::endl;
    
    std::cout<<"Starting stereo calibration .."<<std::endl;
    int fisheyeFlags = 0;
    fisheyeFlags |= cv::fisheye::CALIB_FIX_INTRINSIC;
    //fisheyeFlags &= cv::fisheye::CALIB_CHECK_COND;
    rms = cv::fisheye::stereoCalibrate(left_objpoints, left_imgpoints, right_imgpoints,left_cameraMatrix, left_distCoeffs,
    right_cameraMatrix, right_distCoeffs, cv::Size(right_img.rows,right_img.cols), rotationMatrix, translationVector,
                                              fisheyeFlags, TERMINATION_CRITERIA);
    std::cout<<"Finished stereo calibration"<<std::endl;
    
};
void elp::getFrames(){
    cv::Mat frame,frame2;
    cap >> frame;
    if (frame.empty()) return;
    cv::flip(frame,frame2,1);
    left_img = frame2(cv::Range(0,height), cv::Range(0,width/2)); //240x320
    right_img = frame2(cv::Range(0,height), cv::Range(width/2,width)); //240x320
};
void elp::processDeep(){
};
void elp::create_capture(int id,int w, int h){
    std::cout<<"tmr "<<std::endl;
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
void elp::init_chess(){
    for(int i{0}; i<CHECKERBOARD[1]; i++){
        for(int j{0}; j<CHECKERBOARD[0]; j++)
            aux_objp.push_back(cv::Point3f((float)j*square_size,(float)i*square_size,0));
    }
};