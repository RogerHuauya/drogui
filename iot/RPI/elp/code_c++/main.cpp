#include <iostream>
#include "elp.h"
#include "utils.h"

utils::FPS fps;
// rm -r results*
int main(){
    int id,width,height;
    std::cout<<"id  width height: ";
    std::cin>>id>>width>>height;    
    elp elp1(id,width,height);
    elp1.getCalibrationData();
    return 0;
};