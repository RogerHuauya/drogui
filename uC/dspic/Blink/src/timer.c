#include "timer.h"
#include "serial.h"
#include <stdio.h>
#include <string.h>
// https://ww1.microchip.com/downloads/en/DeviceDoc/70205D.pdf

void initTimer1(int pre, int priority){
    
    T1CONbits.TON = 0;
    T1CONbits.TCKPS = pre;
    T1CONbits.TCS = 0;
    T1CONbits.TGATE = 0;
    TMR1 = 0;
    PR1 = 0xFFFF;
    IPC0bits.T1IP = priority;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;   
    T1CONbits.TON = 1;
}

void initTimer2(int pre, int priority){
    
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = pre;
    T2CONbits.TCS = 0;
    T2CONbits.TGATE = 0;
    TMR2 = 0;
    PR2 = 0xFFFF;
    IPC1bits.T2IP = priority;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;   
    T2CONbits.TON = 1;
}

void initTimer3(int pre, int priority){
    
    T3CONbits.TON = 0;
    T3CONbits.TCKPS = pre;
    T3CONbits.TCS = 0;
    T3CONbits.TGATE = 0;
    TMR3 = 0;
    PR3 = 0xFFFF;
    IPC2bits.T3IP = priority;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;   
    T3CONbits.TON = 1;
}

void initTimer4(int pre, int priority){
    
    T4CONbits.TON = 0;
    T4CONbits.TCKPS = pre;
    T4CONbits.TCS = 0;
    T4CONbits.TGATE = 0;
    TMR4 = 0;
    PR4 = 0xFFFF;
    IPC6bits.T4IP = priority;
    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 1;   
    T4CONbits.TON = 1;
}

void initTimer5(int pre, int priority){
    
    T5CONbits.TON = 0;
    T5CONbits.TCKPS = pre;
    T5CONbits.TCS = 0;
    T5CONbits.TGATE = 0;
    TMR5 = 0;
    PR5 = 0xFFFF;
    IPC7bits.T5IP = priority;
    IFS1bits.T5IF = 0;
    IEC1bits.T5IE = 1;   
    T5CONbits.TON = 1;
}

void initTimer(timer* t, int n, int pre, int priority){
    t -> n = n;
    
    switch (pre){
        case DIV1: t -> prescaler = 1; break;
        case DIV8: t -> prescaler = 8; break;
        case DIV64: t -> prescaler = 64; break;
        case DIV256: t -> prescaler = 256; break;
    }
    switch(n){
        case 1: initTimer1(pre, priority); break;
        case 2: initTimer2(pre, priority); break;
        case 3: initTimer3(pre, priority); break;
        case 4: initTimer4(pre, priority); break;
        case 5: initTimer5(pre, priority); break;
    }
}

void setTimerFrecuency(timer* t, unsigned long long freq){

    uint16_t PR;

    if(freq < 4) PR = 0;
    else if(freq*t->prescaler*2 > FCY) PR = 1; 
    else PR = ( (double) FCY/(freq*t->prescaler) - 1);

    
    switch(t -> n){
        case 1:if(PR < PR1) PR1 = PR, TMR1 = PR-1; 
                else PR1 = PR; break;
        case 2:if(PR < PR2) PR2 = PR, TMR2 = PR-1;
                else PR2 = PR;  break;
        case 3:if(PR < PR3) PR3 = PR, TMR3 = PR-1;
                else PR3 = PR;  break;
        case 4:if(PR < PR4) PR4 = PR, TMR4 = PR-1;
                else PR4 = PR;  break;
        case 5:if(PR < PR5) PR5 = PR, TMR5 = PR-1;
                else PR5 = PR;  break;
    }

}

void clearTimerFlag(timer* t){
    switch(t -> n){
        case 1: IFS0bits.T1IF = 0; break;
        case 2: IFS0bits.T2IF = 0; break;
        case 3: IFS0bits.T3IF = 0; break;
        case 4: IFS1bits.T4IF = 0; break;
        case 5: IFS1bits.T5IF = 0; break;
    }
}
