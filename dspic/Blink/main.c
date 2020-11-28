
#define MAIN
#ifdef MAIN

#include <xc.h>
#include "config.h"
#include <libpic30.h>

#include "io.h"
#include "serial.h"
#include "timer.h"
#include "control.h"
#include "MM7150.h"
#include "pwm.h"
#include "i2c.h"
#include "utils.h"
#include "registerMap.h"
#include "bmp280.h"
char buffer[80];

i2c slave;

pwm m1, m2, m3, m4;

sensor acc, gyro, ori;

pid z_control;
pid x_control;
pid y_control;

pid roll_control;
pid pitch_control;
pid yaw_control;

serial Serial1;

timer readSensors;
timer millis;
timer readPress;

double roll, pitch, yaw, z;
volatile unsigned long long time = 0;

void initializeSystem(){
    
    initConfig();
    
    setPwmPrescaler(0);

    initPwmPin(&m1, PWM3_H);
    setPwmDutyLimits(&m1, 125, 250);
    setPwmFrecuency(&m1, 3500);
    setPwmDutyTime(&m1, 0);
    
    initPwmPin(&m2, PWM4_H);
    setPwmDutyLimits(&m2, 125, 250);
    setPwmFrecuency(&m2, 3500);
    setPwmDutyTime(&m2, 0);

    initPwmPin(&m3, PWM5_H);
    setPwmDutyLimits(&m3, 125, 250);
    setPwmFrecuency(&m3, 3500);
    setPwmDutyTime(&m3, 0);
    
    initPwmPin(&m4, PWM6_H);
    setPwmDutyLimits(&m4, 125, 250);
    setPwmFrecuency(&m4, 3500);
    setPwmDutyTime(&m4, 0);
    
    initPwm();

    initSerial(&Serial1, SERIAL1, 115200);

    initPid(&z_control, 0, 0, 0, 0, 10 , 100);
    initPid(&x_control, 0, 0, 0, 0, 10 , 100);
    initPid(&y_control, 0, 0, 0, 0, 10 , 100);
    
    initPid(&roll_control, 0, 0, 0, 0, 1 , 100);
    initPid(&pitch_control, 0, 0, 0, 0, 1 , 100);
    initPid(&yaw_control, 0, 0, 0, 0, 1 , 100);
    

    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 50, 10);

    initBmp280();

    initTimer(&readSensors, 2, DIV256, 3);
    setTimerFrecuency(&readSensors, 100);

    initTimer(&readPress, 4, DIV256, 3);
    setTimerFrecuency(&readPress, 20);

    initTimer(&millis, 3, DIV256, 3);
    setTimerFrecuency(&millis, 1000);
    
    
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    clearI2Cregisters(I2C2);

    __delay_ms(500);
    
    serialWriteString(&Serial1, "init");
    
}


void getEuler(double q0,double q1,double q2, double q3);

bool caso = true;

long long entrada = 0;
int dig = 0;
void timerInterrupt(2){
    readOrient(&ori);        
    getEuler(ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);

    setReg(ROLL_VAL,(float)(roll));
    setReg(PITCH_VAL,(float)(pitch));
    setReg(YAW_VAL,(float)(yaw));

    
    clearTimerFlag(&readSensors);
}

int32_t raw_press, raw_temp;
float press, temper;
void timerInterrupt(4){
    
    raw_temp = bmpReadTemperature();
    raw_press = bmpReadPressure();
    temper = bmp280CompensateTemperature(raw_temp);
    press = bmp280CompensatePressure(raw_press);
    setReg(RAW_TEMP, raw_temp);
    setReg(TEMP_ABS, temper);
    setReg(RAW_PRESS, raw_press);
    setReg(PRESS_ABS, press);

    //Cálculo de z
    setReg(Z_VAL, z);
    clearTimerFlag(&readPress);
}

void timerInterrupt(3){
    time++;
    clearTimerFlag(&millis);
}

double angle_dif(double angle1, double angle2);

double  H,R,P,Y, Zdes;
double M1,M2,M3,M4;
uint8_t haux = 0;
double roll_off = -3.09995788 , pitch_off = 0.0170128063, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
double roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
long long pm = 0;
enum PIDconstants {ROLL, PITCH, YAW};
double roll_const[5][3] = {{25, 25, 10}, {25,25, 10}, {20, 25, 15}, {20, 25, 15}, {20, 25, 15}};
double pitch_const[5][3] = {{25, 25, 10}, {25,25, 10}, {20, 25, 15}, {20, 25, 15}, {20, 25, 15}};
double yaw_const[5][3] = {{0, 0, 0}, {0,0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
void initPIDconstants(pid* p, int indx){
    switch (indx)
    {
    case ROLL:
        for(int i = 0; i < 5; i ++){
            p->kp[i] = roll_const[i][0];
            p->ki[i] = roll_const[i][1];
            p->kd[i] = roll_const[i][2];
        }
        break;
    case PITCH:
        for(int i = 0; i < 5; i ++){
            p->kp[i] = pitch_const[i][0];
            p->ki[i] = pitch_const[i][1];
            p->kd[i] = pitch_const[i][2];
        }
        break;
    case YAW:
        for(int i = 0; i < 5; i ++){
            p->kp[i] = yaw_const[i][0];
            p->ki[i] = yaw_const[i][1];
            p->kd[i] = yaw_const[i][2];
        }
        break;
    
    default:
        break;
    }
}

int main(void){
    
    initializeSystem();
    int val = 0;
    int Kp, Ki, Kd, roll_d, pitch_d, yaw_d;
    __delay_ms(1000);
    yaw_off = yaw;
    initPIDconstants(&roll_control, ROLL);
    initPIDconstants(&pitch_control, PITCH);
    initPIDconstants(&yaw_control, YAW);
    setReg(PID_INDEX, -1);
    while(1){
        roll_ref = getReg(ROLL_REF) + roll_off;
        pitch_ref = getReg(PITCH_REF) + pitch_off;
        yaw_ref = getReg(YAW_REF) + yaw_off;

        z_ref += fabs(getReg(Z_REF) - z_ref) >= getReg(Z_REF_SIZE)  ? copysign(getReg(Z_REF_SIZE), getReg(Z_REF) - z_ref) : 0;
        
        H = computePid(&z_control, z_ref - z, time) + getReg(Z_MG);
        R = computeIndexedPid(&roll_control, angle_dif( roll_ref, roll), time, H);
        P = computeIndexedPid(&pitch_control, angle_dif( pitch_ref, pitch),time, H);
        Y = computeIndexedPid(&yaw_control, angle_dif(yaw_ref, yaw),time, H);
        
        setReg(ROLL_U, R);
        setReg(PITCH_U, P);
        setReg(YAW_U, Y);
        setReg(Z_U, H);

        
        M1 = H + R - P - Y;
        M2 = H - R - P + Y;
        M3 = H - R + P - Y;
        M4 = H + R + P + Y;
        
        if(getReg(Z_REF) == 0 || (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)){
            setReg(Z_REF, 0);
            H = 0; z_ref = 0;
            M1 = M2 = M3 = M4 = 0;
            int index = getReg(PID_INDEX);
            if(index >= 0) {
                roll_control.kp[index] = getReg(ROLL_KP);
                roll_control.ki[index] = getReg(ROLL_KI);
                roll_control.kd[index] = getReg(ROLL_KD);
                
                pitch_control.kp[index] = getReg(PITCH_KP);
                pitch_control.ki[index] = getReg(PITCH_KI);
                pitch_control.kd[index] = getReg(PITCH_KD);
                
                yaw_control.kp[index] = getReg(YAW_KP);
                yaw_control.ki[index] = getReg(YAW_KI);
                yaw_control.kd[index] = getReg(YAW_KD);

                z_control.kp[0] = getReg(Z_KP);
                z_control.ki[0] = getReg(Z_KI);
                z_control.kd[0] = getReg(Z_KD);
            }
            resetPid(&roll_control, time);
            resetPid(&pitch_control, time);
            resetPid(&yaw_control, time);
            resetPid(&z_control, time);
        }
        
        setReg(ROLL_U, (float) R);
        setReg(PITCH_U, (float) P);
        setReg(YAW_U, (float) Y);

        setPwmDutyTime(&m1, min(max(M1,0), 100));
        setPwmDutyTime(&m2, min(max(M2,0), 100));
        setPwmDutyTime(&m3, min(max(M3,0), 100));
        setPwmDutyTime(&m4, min(max(M4,0), 100));
        
        //sprintf(buffer, "%.3lf %.3lf %.3lf %.3lf\n", H, R, P, Y);
        sprintf(buffer, "%.3lf %.3lf %.3lf\n", roll, pitch, yaw);
        serialWriteString(&Serial1, buffer);
        __delay_ms(max((int) getReg(TS_CONTROL), 5));

    }
    return 0;
}

double angle_dif(double angle1, double angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
        else return angle1 - angle2;
    }
}

void getEuler(double q0, double q1, double q2, double q3){
	
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q0 * q1 + q2 * q3);
    double cosr_cosp = 1 - 2 * (q1 * q1 + q2 * q2);
    roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q0 * q2 - q3 * q1);
    if (abs(sinp) >= 1)
        pitch = copysign(pi / 2, sinp); // use 90 degrees if out of range
    else
        pitch = asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q0 * q3 + q1 * q2);
    double cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
    yaw = atan2(siny_cosp, cosy_cosp);
}
#endif