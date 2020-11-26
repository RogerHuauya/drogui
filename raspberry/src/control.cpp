#include "control.h"
void send_PID_ROLL(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID ROLL) "\n");
    cout<<"Set index:"<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    cout<<"KP KI KD = "<<endl;
    cin>>value1>>value2>>value3;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(ROLL_KP, value1);
    rasp_i2c.sendFloat(ROLL_KI, value2);
    rasp_i2c.sendFloat(ROLL_KD, value3);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return;
}
void send_PID_PITCH(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID PITCH) "\n");
    cout<<"Set index:"<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    cout<<"KP KI KD = "<<endl;
    cin>>value1>>value2>>value3;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PITCH_KP, value1);
    rasp_i2c.sendFloat(PITCH_KI, value2);
    rasp_i2c.sendFloat(PITCH_KD, value3);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return;
}

void send_H(){
    cin_thread=true;
    cls(); 
    float value1,value2;
    printf(green(H value and stepsize) "\n");
    cout<<"H deltaH = "<<endl;
    cin>>value1>>value2;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(H_VAL, value1);
    rasp_i2c.sendFloat(H_STEP_SIZE, value2);
    cout<<"Values sent : "<<endl;
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
    cout<<"Set index:"<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    cout<<"KP KI KD = "<<endl;
    cin>>value1>>value2>>value3;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(YAW_KP, value1);
    rasp_i2c.sendFloat(YAW_KI, value2);
    rasp_i2c.sendFloat(YAW_KD, value3);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return;
}
