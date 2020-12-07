#define CONTROL_LUNG_TEST
#ifdef CONTROL_LUNG_TEST

#include <xc.h>
#include "config.h"
#include "io.h"
#include "serial.h"
#include "piston.h"
#include "sensors.h"
#include "utils.h"
#include "blender.h"
#include "peep.h"
#include "press_reg.h"
#include "control.h"

//#define AUX PRTD, 1
#define TIMER_SENSORS 4, 5
#define TIMER_TIME      6, 7

int state = 0;
unsigned long long time = 0;
double volumen = 0;
double error, control, sob_imp = 0;
unsigned long long est_time = 0, time_zero = 0;
char buffer[80], read_buffer[80];

press lung_press;
//flow lung_flow;

piston my_piston;

timer read_sensors;
timer timer_time;

pid lung_press_control;

void initSystem(){
    
    initPBCLK();
    initSerial(115200, 4);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    pinMode(BOB_FIL, OUTPUT);
    pinMode(BOB_INS, OUTPUT);
    pinMode(BOB_ESP, OUTPUT);

    digitalWrite(BOB_FIL, HIGH);
    digitalWrite(BOB_INS, HIGH);
    digitalWrite(BOB_ESP, HIGH);
    
    initPress(&lung_press, I2C2);
    //initFlow(&lung_flow, I2C1);
    
    setPiston(&my_piston);
    initPistonStepper(PRTC, 1, PRTC , 4);
    initPistonVelTimer(DIV256);
    initPistonAccelTimer(DIV256, 2000);

    
    
    digitalWrite(BOB_FIL, LOW);
    moveHome(&my_piston);
    digitalWrite(BOB_FIL, HIGH);
    digitalWrite(BOB_ESP, LOW);
    digitalWrite(BOB_INS, LOW);
    delayMs(1000);
    digitalWrite(BOB_ESP, HIGH);
    
    initTimer(&read_sensors, TIMER_SENSORS, DIV256);
    setTimerFrecuency(&read_sensors, 100);
    initTimer(&timer_time, TIMER_TIME, DIV256);
    setTimerFrecuency(&timer_time, 1000);


    //initPid(&lung_flow_control, 1, 0, 0, 0, 10, 10);
    //initPid(&piston_press_control, 4, 0, 0, 0, 10, 400);
    initPid(&lung_press_control, 0, 0, 0, 0, 0, 0);
}


void EVAL(timerInterrupt, TIMER_SENSORS){
    readPress(&lung_press);
    sprintf(buffer, "%.1lf %.1lf %.1lf %.1lf %.1lf %.1lf %llu %.2lf\n", \
                        lung_press.val, \
                        lung_press_control.kp, \
                        lung_press_control.ki, \
                        lung_press_control.kd, \
                        lung_press_control.isat,\
                        lung_press_control.osat);
                        //est_time - time_zero,\
                        //sob_imp);
    
    serialWriteString(buffer);
    clearTimerFlag(&read_sensors);
}

void EVAL(timerInterrupt, TIMER_TIME){
    time += 1;
    clearTimerFlag(&timer_time);
}

int op;
double aux;
double kp_aux, ki_aux, kd_aux, isat_aux, osat_aux;

int main(void){
    initSystem();
    int sp = 0;
    resetPid(&lung_press_control, time);
    while(true){
        if(serialAvailable()){
            serialReadString(read_buffer, 1000);
            sscanf(read_buffer, "%d %lf", &op, &aux);
            if(op == 0){
                stopPiston(&my_piston);
                
                digitalWrite(BOB_INS, HIGH);
                digitalWrite(BOB_ESP, LOW);
                digitalWrite(BOB_FIL, LOW);
                
                moveHome(&my_piston); 
                lung_press_control.kp = kp_aux;
                lung_press_control.kd = kd_aux;
                lung_press_control.ki = ki_aux;
                lung_press_control.isat = isat_aux;
                lung_press_control.osat = osat_aux;
                resetPid(&lung_press_control, time); 
                sp = 0;
                
                digitalWrite(BOB_FIL, HIGH);
                digitalWrite(BOB_INS, LOW);
                delayMs(1000);
                digitalWrite(BOB_ESP, HIGH);
            }
            else if(op == 1){   sp = aux;}
            else if(op == 2){   kp_aux = aux;}
            else if(op == 3){   ki_aux = aux;}
            else if(op == 4){   kd_aux = aux;}
            else if(op == 5){   isat_aux = aux;}
            else if(op == 6){   osat_aux = aux;}
        }
        error =  sp - lung_press.val;
        sob_imp = max(sob_imp, -error);

        if(sp == 0){
            sob_imp = 0;
            time_zero = time;
            est_time = time;
        }
        else if(abs(error) >= 0.4){
            est_time = time;
        } 
	//
        control = computePid(&lung_press_control, error, time);
        setVelPiston(&my_piston, control);
        delayMs(10);
    
    }
    return 0;
}
#endif
