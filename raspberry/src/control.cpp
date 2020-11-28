#include "control.h"
#include "registerMap.h"
#include <iostream>
#include "utils.h"

extern bool cin_thread;
extern bool logging_state;

void send_PID_ROLL(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID ROLL) "\n");
    std::cout<<"Set index:"<<std::endl;
    std::cin>>value1;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    std::cout<<"KP KI KD = "<<std::endl;
    std::cin>>value1>>value2>>value3;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(ROLL_KP, value1);
    rasp_i2c.sendFloat(ROLL_KI, value2);
    rasp_i2c.sendFloat(ROLL_KD, value3);
    std::cout<<"Values sent : "<<std::endl;
    sleep(1);
    cin_thread=false;
    return;
}
void send_PID_PITCH(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID PITCH) "\n");
    std::cout<<"Set index:"<<std::endl;
    std::cin>>value1;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    std::cout<<"KP KI KD = "<<std::endl;
    std::cin>>value1>>value2>>value3;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(PITCH_KP, value1);
    rasp_i2c.sendFloat(PITCH_KI, value2);
    rasp_i2c.sendFloat(PITCH_KD, value3);
    std::cout<<"Values sent : "<<std::endl;
    sleep(1);
    cin_thread=false;
    return;
}

void send_H(){
    cin_thread=true;
    cls(); 
    float value1,value2;
    printf(green(H value and stepsize) "\n");
    std::cout<<"H deltaH = "<<std::endl;
    std::cin>>value1>>value2;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(Z_REF, value1);
    rasp_i2c.sendFloat(Z_REF_SIZE, value2);
    std::cout<<"Values sent : "<<std::endl;
    if(value1 == 0) logging_state = false;
    else logging_state = true;
    sleep(1);
    cin_thread=false;
    return; 
}
void send_PID_YAW(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID YAW) "\n");
    std::cout<<"Set index:"<<std::endl;
    std::cin>>value1;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    std::cout<<"KP KI KD = "<<std::endl;
    std::cin>>value1>>value2>>value3;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(YAW_KP, value1);
    rasp_i2c.sendFloat(YAW_KI, value2);
    rasp_i2c.sendFloat(YAW_KD, value3);
    std::cout<<"Values sent : "<<std::endl;
    sleep(1);
    cin_thread=false;
    return;
}

