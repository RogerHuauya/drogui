#include "pwm.h"
short init_done = 0;
int master_prescaler = 0;

void initPwmPin(pwm* p, int n, bool primary){
    
    p -> n = n;
    p -> primary  = primary;
    switch (n){
        case 1:
            PWMCON1bits.ITB = 1;  IOCON1bits.PMOD = 3;  
            if(primary)  IOCON1bits.PENH = 1; else IOCON1bits.PENL = 1; break; 
        case 2:
            PWMCON2bits.ITB = 1;  IOCON2bits.PMOD = 3; 
            if(primary)  IOCON2bits.PENH = 1; else IOCON2bits.PENL = 1; break;
        case 3:
            PWMCON3bits.ITB = 1;  IOCON3bits.PMOD = 3;  
            if(primary)  IOCON3bits.PENH = 1; else IOCON3bits.PENL = 1; break; 
    }
}

void setPwmPrescaler(int prescaler){
    master_prescaler = (1 << prescaler);
    PTCON2bits.PCLKDIV = prescaler;
}

void initPwm(){
    PTCONbits.PTEN = 1;
}


void setPwmFrecuency(pwm* p, double freq){
    uint16_t PR =  FCY/(freq*master_prescaler)*2LL - 1;
    p -> period = PR; 
    switch (p -> n){
        case 1:
            if(p->primary)  PHASE1 = PR; else SPHASE1 = PR; break; 
        case 2:
            if(p->primary)  PHASE2 = PR; else SPHASE2 = PR; break;
        case 3:
            if(p->primary)  PHASE3 = PR; else SPHASE3 = PR; break; 
    }
}




void setPwmDuty(pwm* p, double percent){
    
    uint16_t DC = (p->period)/(100.0/percent);

    switch (p -> n){
        case 1:
            if(p->primary)  PDC1 = DC; else SDC1 = DC; break; 
        case 2:
            if(p->primary)  PDC2 = DC; else SDC2 = DC; break;
        case 3:
            if(p->primary)  PDC3 = DC; else SDC3 = DC; break; 
    }
}
