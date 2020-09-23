#include <iostream>
#include "elp.h"
#include "utils.h" //fps

utils::FPS fps;
// rm -r results*

int main(){
    int id,width=1280,height=480;
    std::cout<<"id ";//width height";
    std::cin>>id;//>>width>>height;    
    elp elp1(id,width,height);
    elp1.getCalibrationData();
    elp1.calibrate();
    return 0;
};