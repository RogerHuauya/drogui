#include "control.h"
#include "registerMap.h"
#include <iostream>
#include "utils.h"

extern bool cin_thread;
extern bool logging_state;

void send_PID_ROLL();
void send_PID_PITCH();    
void send_PID_YAW();
void send_PID_Z();
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
    rasp_i2c.sendFloat(PID_VAR, PID_ROLL);

    std::cout<<"KP KI KD = "<<std::endl;
    std::cin>>value1>>value2>>value3;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(ROLL_KP, value1);
    rasp_i2c.sendFloat(ROLL_KI, value2);
    rasp_i2c.sendFloat(ROLL_KD, value3);
    std::cout<<"Values sent : "<<std::endl;
    sleep(1);
    //rasp_i2c.sendFloat(PID_VAR, -1);

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
    rasp_i2c.sendFloat(PID_VAR, PID_PITCH);
    std::cout<<"KP KI KD = "<<std::endl;
    std::cin>>value1>>value2>>value3;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(PITCH_KP, value1);
    rasp_i2c.sendFloat(PITCH_KI, value2);
    rasp_i2c.sendFloat(PITCH_KD, value3);
    std::cout<<"Values sent : "<<std::endl;
    sleep(1);
    //rasp_i2c.sendFloat(PID_VAR, -1);
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
    rasp_i2c.sendFloat(PID_VAR, PID_YAW);
    std::cout<<"KP KI KD = "<<std::endl;
    std::cin>>value1>>value2>>value3;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(YAW_KP, value1);
    rasp_i2c.sendFloat(YAW_KI, value2);
    rasp_i2c.sendFloat(YAW_KD, value3);
    std::cout<<"Values sent : "<<std::endl;
    sleep(1);
    //rasp_i2c.sendFloat(PID_VAR, -1);
    return;
}

void send_PID_Z(){    
    cls(); 
    float value1,value2,value3;
    printf(green(PID Z) "\n");

    rasp_i2c.sendFloat(PID_INDEX, value1);
    rasp_i2c.sendFloat(PID_VAR, PID_Z);
    printf("KP KI KD = \n");
    std::cin>>value1>>value2>>value3;

    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(Z_KP, value1);
    rasp_i2c.sendFloat(Z_KI, value2);
    rasp_i2c.sendFloat(Z_KD, value3);
    printf("The values have been sent over I2C\n");
    sleep(1);
    //rasp_i2c.sendFloat(PID_VAR, -1);
    return;
}
enum REF_OP {REF_ROLL, REF_PITCH, REF_YAW, REF_X, REF_Y, REF_Z};
char ref_op[6][40] = {"ref_roll delta_roll_ref\n", "ref_pitch delta_pitch_ref\n", "ref_yaw delta_yaw_ref\n", \
                        "ref_x delta_x_ref\n", "ref_y delta_y_ref\n", "ref_z delta_z_ref\n"};
void send_ref(){
    cin_thread = true;
    //sleep(1000);
    cls();
    int op;
    printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
    printf(green([0]) " " white(ROLL REF\n));
    printf(green([1]) " " white(PITCH REF\n));
    printf(green([2]) " " white(YAW REF\n));
    printf(green([3]) " " white(X REF\n));
    printf(green([4]) " " white(Y REF\n));
    printf(green([5]) " " white(Z REF\n));
    std::cin>>op;
    float value1,value2;
    printf(green(%s) "\n", ref_op[op]);
    std::cin >> value1 >> value2;
    if(std::cin.fail()) throw 505;
    switch(op){
        case REF_ROLL: rasp_i2c.sendFloat(ROLL_REF, value1*pi/180.0); rasp_i2c.sendFloat(ROLL_REF_SIZE, value2*pi/180.0); break;
        case REF_PITCH: rasp_i2c.sendFloat(PITCH_REF, value1*pi/180.0); rasp_i2c.sendFloat(PITCH_REF_SIZE, value2*pi/180.0); break;
        case REF_YAW: rasp_i2c.sendFloat(YAW_REF, value1*pi/180.0); rasp_i2c.sendFloat(YAW_REF_SIZE, value2*pi/180.0); break;
        case REF_X: rasp_i2c.sendFloat(X_REF, value1); rasp_i2c.sendFloat(X_REF_SIZE, value2); break;
        case REF_Y: rasp_i2c.sendFloat(Y_REF, value1); rasp_i2c.sendFloat(Y_REF_SIZE, value2); break;
        case REF_Z: rasp_i2c.sendFloat(Z_REF, value1); rasp_i2c.sendFloat(Z_REF_SIZE, value2); break;
    }

    std::cout<<"Values have been sent over I2C : "<<std::endl;
    if(value1 == 0) logging_state = false;
    else logging_state = true;
    sleep(1);
    cin_thread = false;
    return; 
     
}
void send_comp_mg(){
    cin_thread = true;
    printf("Enter Compensation:\n");
    float mg;
    std::cin>>mg;
    rasp_i2c.sendFloat(Z_MG, mg);
    cin_thread = false;
}
