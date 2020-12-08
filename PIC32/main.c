//#define MAIN
#ifdef MAIN

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

#define TIMER_SENSORS 4, 5
#define TIMER_COMMAND 5, 6
#define TIMER_TIME      6, 7
#define TIMER_PERIPHERAL 7, 1

enum states {STAND_BY, ASTERISCO, CONTROL_LUNG, INSPIRATION, ESPIRATION, CONTROL_PISTON,
            ESPONTANEO_SIMV, ESPONTAENO_CPAP, INSPIRATION_PS};
enum ControlVariables { PRESS, VOLUMEN};
int state = 0;
unsigned long long time = 0, time_simv;
double volumen = 0;

flow lung_flow;
press piston_press;
press lung_press;

piston my_piston;

blender blendi;
peep pip;
press_reg preg;

timer read_sensors;
timer read_command;
timer timer_time;
timer timer_peripheral;

pid lung_press_control;
pid lung_flow_control;
pid piston_press_control;



void initSystem(){
    
    initPBCLK();
    initSerial(115200, 4);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    pinMode(BOB_ESP, OUTPUT);
    pinMode(BOB_FIL, OUTPUT);
    pinMode(BOB_INS, OUTPUT);

    digitalWrite(BOB_ESP, HIGH);
    digitalWrite(BOB_FIL, HIGH);
    digitalWrite(BOB_INS, HIGH);
    
    initFlow(&lung_flow, I2C2);
    initPress(&piston_press, I2C2);
    initPress(&lung_press, I2C1);
    
    initBlender(&blendi, I2C1, I2C4, PRTC, 1, PRTD, 1);
    initPeep(&pip, PRTC, 2, PRTE, 8);
    setPeepPress(&pip, &lung_press);
    initPressReg(&preg, PRTE, 2, PRTE, 4);
    setPressRegPress(&preg, &piston_press);
    
    setPiston(&my_piston);
    initPistonStepper(PRTC, 4, PRTC , 1);
    initPistonVelTimer(DIV256);
    initPistonAccelTimer(DIV256, 2000);

    digitalWrite(BOB_FIL, LOW);
    moveHome(&my_piston);
    digitalWrite(BOB_FIL, HIGH);
    
    initTimer(&read_sensors, TIMER_SENSORS, DIV256);
    setTimerFrecuency(&read_sensors, 200);
    initTimer(&read_command, TIMER_COMMAND, DIV256);
    setTimerFrecuency(&read_command, 500);
    initTimer(&timer_time, TIMER_TIME, DIV256);
    setTimerFrecuency(&timer_time, 1000);
    initTimer(&timer_peripheral, TIMER_PERIPHERAL, DIV256);

    initPid(&lung_flow_control, 1, 0, 0, 0, 10, 10);
    initPid(&piston_press_control, 1, 0, 0, 0, 10, 10);
    initPid(&lung_press_control, 1, 0, 0, 0, 10, 10);
}


void EVAL(timerInterrupt, TIMER_SENSORS){
    readPress(&lung_press);
    readPress(&piston_press);

    volumen += lung_flow.val / 400;
    readFlow(&lung_flow);
    volumen += lung_flow.val / 400;
    
    clearTimerFlag(&read_sensors);
}

void EVAL(timerInterrupt, TIMER_COMMAND){
    
    if(serialAvailable()){
        /* TODO: leer comando*/
    }
    
    clearTimerFlag(&read_command);
}

void EVAL(timerInterrupt, TIMER_TIME){
    time += 1;
    time_simv += 1;
    clearTimerFlag(&timer_time);
}

void EVAL(timerInterrupt, TIMER_PERIPHERAL){
    if(abs(peep2Pos(getPEEP()) - getPeepPos(&pip)) > epsPeep )
        stepPeep(&pip, peep2Pos(getPEEP()) > getPeepPos(&pip) ? FWD : BWD);

    if(abs(pressReg2Pos(80) - getPressRegPos(&preg)) > epsPressReg )
        stepPressReg(&preg, pressReg2Pos(80) > getPressRegPos(&preg) ? FWD : BWD);
    
    if(abs(FIO22Pos(getFIO2()) - getBlenderPos(&blendi)) > epsBlender )
        stepPeep(&pip, FIO22Pos(getFIO2()) > getBlenderPos(&blendi) ? FWD : BWD);
    
    clearTimerFlag(&timer_peripheral);
}



void changeState(int stat){
    state = stat;
    
    if(state == INSPIRATION) 
        time = volumen = 0, resetPid(&lung_flow_control, 0), resetPid(&lung_press_control, 0), 
        digitalWrite(BOB_ESP, HIGH), digitalWrite(BOB_INS, LOW); 
    
    if(state == ESPIRATION)
        digitalWrite(BOB_INS, HIGH), digitalWrite(BOB_ESP, LOW), digitalWrite(BOB_FIL, LOW);
    
    if(state == ASTERISCO)
        readPeep(&pip);
}


void asterisco(){

    switch (getMODE()){
        
        case SIMV_P: case SIMV_V:

            if(time_simv >= getTRIGGERWINDOW()) changeState(INSPIRATION);
            if(getTRIGGER() == PRESS)
                if(lung_press.val <= getPRESSTRIGGER()) changeState(INSPIRATION);
            else
                if(lung_flow.val >= getFLOWTRIGGER()) changeState(INSPIRATION);
            break;

        case PSV:    
            if(getTRIGGER() == PRESS)
                if(lung_press.val <= getPRESSTRIGGER()) changeState(INSPIRATION_PS);
            else
                if(lung_flow.val >= getFLOWTRIGGER()) changeState(INSPIRATION_PS);
            break;

        case CMV_V: case CMV_P:
            if(time >= getTRIGGERWINDOW()) changeState(INSPIRATION);
            break;

        case ACMV_P: case ACMV_V:
            
            if(time >= getTRIGGERWINDOW()) changeState(INSPIRATION);
            if(getTRIGGER() == PRESS)
                if(lung_press.val <= getPRESSTRIGGER()) changeState(INSPIRATION);
            else
                if(lung_flow.val >= getFLOWTRIGGER()) changeState(INSPIRATION);
            break;

        case CPAP:
            if(lung_press.val <= getCPAP() - 2) changeState(ESPONTAENO_CPAP);
            break;

        default:
            break;
    }

}



void inspiration(){
        
    if(getCONTROLVARIABLE() == PRESS){
        if(time >= getTI()) changeState(ESPIRATION);
        double desired_press = min(time, getSLOPE())*(getPRESS() - getPEEP())/getSLOPE() + getPEEP();
        setVelPiston(&my_piston, computePid(&lung_press_control, getPRESS() - lung_press.val, time));
    }
    else{
        if(time >= getTI()) changeState(ESPIRATION);
        if(volumen >= getVTI()){ stopPiston(&my_piston); return;}
        setVelPiston(&my_piston, computePid(&lung_flow_control, getFLOW() - lung_flow.val, time));
    }
}

void espiration(){

    if(getPos(&my_piston) > 3){
        setVelPiston(&my_piston, 10*(3 - getPos(&my_piston))); 
        return;
    }

    if(piston_press.val < 80)  return;

    digitalWrite(BOB_FIL, HIGH);
    moveHome(&my_piston);
    
    while(lung_flow.val < -2); 
    
    changeState(ASTERISCO);
}

void inspirationPs(){

    if(abs(lung_press.val - getPS()) <= 1)
        if(lung_flow.val <= getFLOWCICLADO()) 
            changeState(ESPIRATION);
    
    double desired_press = min(time, getSLOPE_PS())*(getPS() - getPEEP())/getSLOPE_PS() + getPEEP();
    setVelPiston(&my_piston, computePid(&lung_press_control, getPS() - lung_press.val, time));
}

void espontaneoSimv(){

}

void espontaneoCpap(){

}

int main(void){
    
    while(true){
        switch(state){
            startDelay();
            case STAND_BY: break;
            case ASTERISCO: asterisco(); break;
            case CONTROL_LUNG: controlLung(); break;
            case INSPIRATION: inspiration(); break;
            case ESPIRATION: espiration(); break;
            case INSPIRATION_PS: inspirationPs(); break;
            case ESPONTAENO_CPAP: espontaneoCpap(); break;
            case ESPONTANEO_SIMV: espontaneoSimv(); break;
            endDelayMs()
        }
    }
    return 0;
}

#endif
