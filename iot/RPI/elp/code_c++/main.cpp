#include <iostream>
#include "elp.h"

int main(){
    int id,width,height;
    std::cout<<"id  width height: ";
    std::cin>>id>>width>>height;    
    elp elp1;
    elp1.create_capture(id,width,height);
    
    while(1){
        elp1.getFrames();
        cv::imshow( "camera1", elp1.left_img );
        cv::imshow( "camera2", elp1.right_img);
        
        char k=(char) cv::waitKey(25);
        if (k == 'q' || k == 'Q') break;
        
    };
    elp1.cap.release();
    elp1.destroyAllWindows();
    return 0;
};