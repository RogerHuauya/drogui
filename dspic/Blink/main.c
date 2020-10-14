#define MAIN
#ifdef MAIN
#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "serial.h"
#include "timer.h"
#include "piston.h"
#include "control.h"
#include "pwm.h"
#include "i2c.h"
#include "utils.h"
#include "registerMap.h"

char buffer[80];

i2c slave;

pwm m1, m2, m3, m4;

pid z_control;
pid x_control;
pid y_control;

pid roll_control;
pid pitch_control;
pid yaw_control;

serial Serial1, Serial2;

timer readSensors;
timer millis;

double roll, pitch, yaw;
volatile unsigned long time = 0;

void initializeSystem(){
    
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    initSerial(&Serial2, SERIAL2, 115200);

    initPid(&z_control, 1, 0, 0, 0, 10 , 100);
    initPid(&x_control, 1, 0, 0, 0, 10 , 100);
    initPid(&y_control, 1, 0, 0, 0, 10 , 100);
    
    initPid(&roll_control, 1, 0, 0, 0, 10 , 100);
    initPid(&pitch_control, 1, 0, 0, 0, 10 , 100);
    initPid(&yaw_control, 1, 0, 0, 0, 10 , 100);
    
    
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
    
    initTimer(&readSensors, 2, DIV256, 6);
    serialFlush(&Serial2);
    setTimerFrecuency(&readSensors, 100);
    

    initTimer(&millis, 3, DIV256, 7);
    setTimerFrecuency(&millis, 1000);
    
    
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    i2c2Reg[0x01] = 0;

    __delay_ms(500);
    
    serialWriteString(&Serial1, "init");
    
}

int sumDigits(long long x){
  int ans = 0;
  while(x > 0){
    ans += x%10;
    x/=10;
  }
  return ans;
}

bool caso = true;

long long entrada = 0;
int dig = 0;
void timerInterrupt(2){
    if(U2STAbits.OERR){ 
        U2STAbits.OERR = 0;
        serialFlush(&Serial2);
    }
    while(serialAvailable(&Serial2)){
        char c = serialReadChar(&Serial2);
      //  serialWriteChar(&Serial1,c);
    //}
        if(c == '$'){
            if(dig == sumDigits(entrada)){    
                roll = 1.0*(entrada%360)/180*pi - pi;
                pitch = 1.0*((entrada/360)%360)/180*pi -pi;
                yaw = 1.0*((entrada/(360LL*360))%360)/180*pi -pi;
            }    
            sprintf(buffer, "%.3lf %.3lf %.3lf\n",  roll, pitch, yaw);
            //sprintf(buffer, "%lld %d %d\n", entrada, dig, sumDigits(entrada));
            serialWriteString(&Serial1, buffer);
            entrada = dig = 0;
            caso = true;
        }
        else if(c == '#'){
            caso = false;
        }
        else{
            if(caso)
                entrada = entrada * 10 + (int)(c - '0');
            else{
                dig = dig *10 +(int) (c-'0');
            }
        }
    }

    clearTimerFlag(&readSensors);
}

void timerInterrupt(3){
    time++;
    clearTimerFlag(&millis);
}
double angle_dif(double angle1, double angle2);

int  H,R,P,Y;
int M1,M2,M3,M4;


int main(void){
    
    initializeSystem();
    int val = 0;
    int Kp, Ki, Kd, roll_d, pitch_d, yaw_d;
    
    while(1){
       /*
        roll +1 +4 -2 -3 pi
        
        pitch +4 +2 -1 -3 0

        yaw   +1 +2 -3 -4 pi

       */
      
  /*      roll_control.Kp = i2c2Reg[PID_ROLL_KP];
      Ki = i2c2Reg[PID_ROLL_KI];        
        Kd = i2c2Reg[PID_ROLL_Kd];

        roll_d = i2c2Reg[ROLL_DEG];
        pitch_d = i2c2Reg[PITCH_DEG];
        yaw_d = i2c2Reg[YAW_DEG];
    */
        H = (double) i2c2Reg[0x05];
        R = computePid(&roll_control, angle_dif(pi, roll), time);
        P = computePid(&pitch_control, angle_dif(0, pitch), time);
        Y = computePid(&yaw_control, angle_dif(pi, yaw), time);
        
        
        R = P = Y = 0;
        M1 = H + R - P + Y;
        M2 = H - R + P + Y;
        M3 = H - R - P - Y;
        M4 = H + R + P - Y;
        
        setPwmDutyTime(&m1, min(max(M1,0), 100));
        setPwmDutyTime(&m2, min(max(M2,0), 100));
        setPwmDutyTime(&m3, min(max(M3,0), 100));
        setPwmDutyTime(&m4, min(max(M4,0), 100));
        //serialWriteString(&Serial1, "hola\n");
        __delay_ms(100);

    }
    return 0;
}

double angle_dif(double angle1, double angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle1 + angle2;
        else return angle1 - angle2;
    }
}
#endif