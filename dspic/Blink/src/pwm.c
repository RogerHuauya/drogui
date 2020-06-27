#include "pwm.h"
short init_done = 0;
/*
void initPwm1(int prescaler, int postscaler){
    P1TCONbits.PTEN = 1;
    P1TCONbits.PTOPS = postscaler;
    P1TCONbits.PTCKPS = prescaler;
    P1TMRbits.PTDIR = 1; // Counting up

}

void initPwm2(int prescaler, int postscaler){
    P2TCONbits.PTEN = 1;
    P2TCONbits.PTOPS = postscaler;
    P2TCONbits.PTCKPS = prescaler;
    P2TMRbits.PTDIR = 1; // Counting up
}

void initPwm(pwm* p, int mod, int n, int prescaler, int postscaler){
    p -> mod = mod;
    p -> n = n;
    p -> prescaler = prescaler;
    p -> postscaler = postscaler;
    if( (init_done & (1 << (mod))) == 0){
        switch (mod){
            case 1: initPwm1(prescaler, postscaler); break;
            case 2: initPwm2(prescaler, postscaler); break;
        }
        init_done |= 1 << (mod); 
    }
    
    switch (n){
        case 1:
            if(mod == 1)  PWM1CON1bits.PEN1H = 1, PWM1CON1bits.PEN1L = 1; 
            else   PWM2CON1bits.PEN1H = 1, PWM2CON1bits.PEN1L = 1; break; 
        case 2:
            if(mod == 1)  PWM1CON1bits.PEN2H = 1, PWM1CON1bits.PEN2L = 1; break;
            //else   PWM2CON1bits.PEN1H = 1, PWM2CON1bits.PEN1L = 1; break; 
        case 3:
            if(mod == 1)  PWM1CON1bits.PEN3H = 1, PWM1CON1bits.PEN3L = 1; break; 
            //else   PWM2CON1bits.PEN1H = 1, PWM2CON1bits.PEN1L = 1; break; 
        //case 4:
        //    if(p->mod == 1) P1DC4 = DC; else P2DC4 = DC; break;
    }
}

void setPwmFrecuency(pwm* p, int freq){
    int PR = (int) FCY/(freq*p->prescaler*2) - 1;
    p -> period = PR;
    switch (p -> mod){
        case 1: P1TPER  = PR; break;
        case 2: P2TPER  = PR; break;
    }
}

void setPwmDuty(pwm* p, double percent){
    
    int DC = (int) percent/100*(p -> period);
    switch (p -> n){
        case 1: if(p->mod == 1) P1DC1 = DC; else P2DC1 = DC; break; 
        case 2: if(p->mod == 1) P1DC2 = DC; /*else P2DC2 = DC;*/ /*break;
        case 3: if(p->mod == 1) P1DC3 = DC; /*else P2DC3 = DC;*//* break;
        //case 4: if(p->mod == 1) P1DC4 = DC; else P2DC4 = DC; break;
    }
}
*/