#include "elp.h"


elp::elp(int id_, int w_,int h_){
    id = id_;
    w = w_;
    h = h_;
    create_capture(id,w,h);
    init_chess();
    flag |= CV_CALIB_FIX_K4;
    flag |= CV_CALIB_FIX_K5;

};
void elp::getCalibrationData(){
    create_folders();
    int c = 0;
    while(1){
        fps.start();
        getFrames();
        cv::Scalar color = cv::Scalar(0,0,255);
        std::vector<cv::Point2f> left_cor = getCornersPoints(&left.img);
        std::vector<cv::Point2f> right_cor = getCornersPoints(&right.img);
        
        if (left_cor.size()==54 && right_cor.size()==54){
            color = cv::Scalar(255,0,0);
            left.objpoints.push_back(aux_objp);
            left.imgpoints.push_back(left_cor);
            right.objpoints.push_back(aux_objp);
            right.imgpoints.push_back(right_cor);
            c++;
        }
        cv::putText(left.img,fps.get()+" :"+std::to_string(c),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::putText(right.img,fps.get()+" :"+std::to_string(c),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::imshow( "camera1", left.img);
        cv::imshow( "camera2", right.img);
        if (waitKey()) break;
    };
    std::cout<<" Total images captured : "<<c<<std::endl;
    cap.release();
    destroyAllWindows();
};
bool elp::create_folder(std::string name){
	if (mkdir(name.c_str(), 0777) == -1){
        std::cout<<"Folder "<<name<<" exits"<<std::endl;
        return false;
    }
	else std::cout <<name<<" Directory created"<<std::endl;
    return true;
};
double elp::errorcalibration_camera(struct subcamera& cam){
  std::vector< cv::Point2f > imagePoints2;
  int i, totalPoints = 0;
  double totalErr = 0, err;
  for (i = 0; i < (int)cam.objpoints.size(); ++i) {
    cv::projectPoints(cv::Mat(cam.objpoints[i]),cam.Rvecs[i],cam.Tvecs[i],cam.cameraMatrix,cam.distCoeffs,imagePoints2);
    err = cv::norm(cv::Mat(cam.imgpoints[i]), cv::Mat(imagePoints2), CV_L2);
    int n = (int)cam.objpoints[i].size();
    totalErr += err*err;
    totalPoints += n;
  }
  return std::sqrt(totalErr/totalPoints);
};
void elp::calibrate_camera(struct subcamera& cam,std::__cxx11::string name){
    cv::Size DIM = cv::Size(cam.img.rows,cam.img.cols);
    //std::cout<<" size "+name<<cam.img.size()<<DIM<<cam.objpoints.size()<<" | "<<cam.imgpoints.size()<<std::endl;
    cv::calibrateCamera(cam.objpoints,cam.imgpoints,cam.img.size(),cam.cameraMatrix,
    cam.distCoeffs,cam.Rvecs,cam.Tvecs,flag,TERMINATION_CRITERIA);
    std::cout<<"error :"<<errorcalibration_camera(cam)<<std::endl;
    const std::string filename = FOLDER+"/individual/"+name;
    cv::FileStorage fs1(filename, cv::FileStorage::WRITE);
    if (fs1.isOpened()){
        fs1 << "cameraMatrix" << cam.cameraMatrix
         << "distCoeffs" << cam.distCoeffs 
         << "objpoints" << cam.objpoints 
         << "imgpoints" << cam.imgpoints
         << "board_height"<< CHECKERBOARD[1]
         << "board_width"<<CHECKERBOARD[0]
         << "square_size" << square_size;
    }
};
void elp::calibrate(){
    std::cout<<"Starting left calibration .."<<std::endl;
    calibrate_camera(left,"calibration_camera_left.yml");
    std::cout<<"Starting right calibration .."<<std::endl;
    calibrate_camera(right,"calibration_camera_right.yml");
    std::cout<<"Finished calibration."<<std::endl;
    cv::Mat R, F, E;
    cv::Vec3d T;
    std::cout<<"Starting stereo calibration .."<<std::endl;
    cv::stereoCalibrate(left.objpoints,left.imgpoints,right.imgpoints,left.cameraMatrix,left.distCoeffs,
    right.cameraMatrix,right.distCoeffs,right.img.size(),R,T,E,F,flag,TERMINATION_CRITERIA);
    std::cout<<"Finished stereo calibration"<<std::endl;
    std::cout<<"Rectification stereo calibration"<<std::endl;
    cv::stereoRectify(left.cameraMatrix,left.distCoeffs,right.cameraMatrix,right.distCoeffs,right.img.size(),
    R,T,R1,R2,P1,P2,Q,flag,1,left.img.size(),&left.roi,&right.roi);
    std::cout<<"Finished rectification"<<std::endl;
    undistort(left);
    undistort(right);
    const std::string filename = FOLDER+"/stereo/total_calibration.yml";
    cv::FileStorage fs1(filename, cv::FileStorage::WRITE);
    if (fs1.isOpened()){
        fs1 << "leftMapX" << left.mapx
         << "leftMapY" << left.mapy 
         << "leftROI" << left.roi
         << "rightMapX" << right.mapx
         << "rightMapY"<< right.mapy
         << "rightROI"<< right.roi
         << "img_size" << left.img.size();
    }
    /*
    stereoMatcher.setMinDisparity(4);
    stereoMatcher.setNumDisparities(128);
    stereoMatcher.setBlockSize(21);
    stereoMatcher.setROI1(left.roi);
    stereoMatcher.setROI2(right.roi);
    stereoMatcher.setSpeckleRange(16);
    stereoMatcher.setSpeckleWindowSize(45);
    */
};
void elp::undistort(struct subcamera& cam){
    cv::initUndistortRectifyMap(cam.cameraMatrix,cam.distCoeffs, R1, P1, cam.img.size(), CV_32F, cam.mapx,cam.mapy);
};
void elp::getFrames(){
    cv::Mat frame,frame2;
    cap >> frame;
    if (frame.empty()) return;
    cv::flip(frame,frame2,1);
    left.img = frame2(cv::Range(0,height), cv::Range(0,width/2)); //240x320
    right.img = frame2(cv::Range(0,height), cv::Range(width/2,width)); //240x320
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
    create_folder(FOLDER+"/individual");
    create_folder(FOLDER+"/stereo");    
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
void elp::see_rectify(){
    create_capture(id,w,h);

    while(1){
        fps.start();
        getFrames();
        cv::Mat left_,right_;
        cv::Scalar color = cv::Scalar(0,255,0);
        cv::remap(left.img,left_, left.mapx, left.mapy, cv::INTER_LINEAR);
        cv::remap(right.img,right_,right.mapx,right.mapy, cv::INTER_LINEAR);
        
        cv::Mat left_gray,right_gray,depth;
        cv::cvtColor(left_,left_gray,CV_BGR2GRAY);
        cv::cvtColor(right_,right_gray,CV_BGR2GRAY);
        stereoMatcher->compute(left_gray,right_gray,depth);
        cv::putText(left_,fps.get(),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::putText(right_,fps.get(),cv::Point(20,20), font, 0.6,color,1, 16 /*CV_AA*/);
        cv::imshow( "camera1", left_);
        cv::imshow( "camera2", right_);
        cv::imshow( "camera3", depth/2048);
        
        if (waitKey()) break;
    };
    cap.release();
    destroyAllWindows();
}