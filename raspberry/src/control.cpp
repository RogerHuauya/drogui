#include "control.h"
#include "registerMap.h"
#include <iostream>
#include "utils.h"

extern bool cin_thread;
extern bool logging_state;
void send_PID(){
    cin_thread = true;
    cls();
    int op;
    printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
    printf(green([0]) " " white(PID_ROLL\n));
    printf(green([1]) " " white(PID_PITCH\n));
    printf(green([2]) " " white(PID_YAW\n));
    printf(green([3]) " " white(PID_Z\n));
    std::cin>>op;
    switch (op){
        case 0: send_PID_ROLL(); break;
        case 1: send_PID_PITCH(); break;
        case 2: send_PID_YAW(); break;
        case 3: send_PID_Z(); break;
    }
    cin_thread = false; 
}
void send_PID_ROLL(){    
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
    return;
}
void send_PID_PITCH(){    
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
    return;
}


void send_PID_YAW(){    
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
    return;
}

void send_PID_Z(){    
    cls(); 
    float value1,value2,value3;
    printf(green(PID Z) "\n");

    rasp_i2c.sendFloat(PID_INDEX, value1);
    printf("KP KI KD = \n");
    std::cin>>value1>>value2>>value3;

    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(Z_KP, value1);
    rasp_i2c.sendFloat(Z_KI, value2);
    rasp_i2c.sendFloat(Z_KD, value3);
    printf("The values have been sent over I2C\n");
    sleep(1);
    return;
}

void send_Zref(){
    cls(); 
    float value1,value2;
    printf(green(Z ref and Z stepsize) "\n");
    std::cout<<"Zref deltaZref = "<<std::endl;
    std::cin>>value1>>value2;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(Z_REF, value1);
    rasp_i2c.sendFloat(Z_REF_SIZE, value2);
    std::cout<<"Values have been sent over I2C : "<<std::endl;
    if(value1 == 0) logging_state = false;
    else logging_state = true;
    sleep(1);
    return; 
}
