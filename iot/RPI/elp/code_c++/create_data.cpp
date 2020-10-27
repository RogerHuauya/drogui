/*
#include "opencv2/opencv.hpp"
#include <iostream>
#include "utils.h"
utils::FPS fps;

int font = cv::FONT_HERSHEY_SIMPLEX;

int id_video,rows,cols,cnt=0,cnt_save=0;
std::string PATH_IMAGES = "images1/" ;
int height,width;
int main(){
  std::cout<<"id camera  = ";
  std::cin>>id_video;
  cv::VideoCapture cap(id_video); 

  cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,480); 
  width  = cap.get(3);
  height = cap.get(4);
  std::cout<<"height: "<<height<<" width: "<<width<<std::endl;
  if(!cap.isOpened()){
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }

  cv::Mat frame,camera1,camera2;
	
  while(1){
    fps.start();  
    cap >> frame;
    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    cv::Size sz = frame.size();
    rows = sz.height; //240
    cols = sz.width;  //640
    //cout<<rows<<" "<<cols<<endl;
    //cout<<sz<<frame.channels()<<endl;
    camera1 = frame(cv::Range(0,rows), cv::Range(0,cols/2)); //240x320
    camera2 = frame(cv::Range(0,rows), cv::Range(cols/2,cols)); //240x320
    char k=(char) cv::waitKey(25);
    if (k == 'q' || k == 'Q') break;
    cnt++;
    if(cnt%8 == 0){
      cnt_save++;
      cv::imwrite(PATH_IMAGES+"left/"+std::to_string(cnt_save)+".jpg", camera1);
      cv::imwrite(PATH_IMAGES+"right/"+std::to_string(cnt_save)+".jpg", camera2);        
      cv::imwrite(PATH_IMAGES+"stereo/"+std::to_string(cnt_save)+".jpg", frame);
      std::cout<<"save images : "+std::to_string(cnt_save)<<std::endl;
    }
    
    // Display the resulting frame
    //cv::putText(camera1,fps.get(), cv::Point(20,20), font, 0.6, cv::Scalar(0,0,255),1, 16 /*CV_AA)
    /*
    cv::putText(camera2,fps.get(), cv::Point(20,20), font, 0.6, cv::Scalar(0,0,255),1, 16 /*CV_AA
    cv::imshow( "camera1", camera1 );
    cv::imshow( "camera2", camera2 );
    if (cnt==512) break;
  }
 
  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  cv::destroyAllWindows();
	
  return 0;
}
*/