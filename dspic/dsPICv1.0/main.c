#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"
#include "serial.h"
#include "analog.h"
#include "control.h"
#include "sensors.h"
#include "stepper.h"
#include "pwm.h"

enum analog_pins {PIN_PRESS_PISTON, PIN_PRESS_LUNG, PIN_FLOW_LUNG};

sensorP* lung;
sensorP* piston;
sensorQ* flow;

pwm* valve;

int control_mode = 1, control_var = 1;
enum control_var {VOL, PRESS};
enum control_modes {SIMV, CMV, ACV, PSV};


/*
Variable controlada: Presion, Volumen
Modo de control: SIMV, CMV, ACV, PSV
*/

void setup(){
    initSerial();
    initAnalog();
    initSensorP(lung, PIN_PRESS_LUNG, 0);
    initSensorP(piston, PIN_PRESS_PISTON, 0);
    initSensorQ(flow, PIN_FLOW_LUNG, 0);   
    initPwm(valve, 1, 1, 0, 0);
}

// tm, ti, tri, pd
void control(int control_var){
    /*
    setVel();
    */
}
bool trigger(int control_mode){
    /*
    if(flujo)
    if(time)
    */
}
void interruptRx(){
    /*TODO: set parameters changed via serial*/
    /*ti, rei, perilla*/

}
void expiration(){
    while(true){
        control(control_var); 
        if(trigger(control_mode)) break;
    }
    stopStepper();
}



void inspiration(){

    while(true){
        setVelStepper(5);
    }
    
}

void timer0(){
    readSensorP(lung);
    readSensorP(piston);
    readSensorQ(flow);
}

int main(void){
    setup();
    while (true){
        char buff[50];
        inspiration();
        expiration();
        sprintf(buff,"Lung press: %.2f \t \
                        Lung flow: %.2f \
                        Piston press: %.2f\n",\
                        lung->val, flow -> val,  piston -> val);
                    
        serialWriteString(buff, 50);
    }
    return 0;
}
