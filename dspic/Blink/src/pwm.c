#include "pwm.h"
#include "serial.h"

short init_done = 0;
int master_prescaler = 0;

void initPwmPin(pwm* p, int n){
    
    p -> n = n;
    switch (n){
        case PWM1_H:    PWMCON1bits.ITB = 1;  IOCON1bits.PMOD = 3;
                        IOCON1bits.PENH = 1, IOCON1bits.POLH = 1; break;
        case PWM1_L:    PWMCON1bits.ITB = 1;  IOCON1bits.PMOD = 3;
                        IOCON1bits.PENH = 1, IOCON1bits.POLH = 0; break;
        
        case PWM2_H:    PWMCON2bits.ITB = 1;  IOCON2bits.PMOD = 3;
                        IOCON2bits.PENH = 1, IOCON2bits.POLH = 1; break;
        case PWM2_L:    PWMCON2bits.ITB = 1;  IOCON2bits.PMOD = 3;
                        IOCON2bits.PENH = 1, IOCON2bits.POLH = 0; break;
        
        case PWM3_H:    PWMCON3bits.ITB = 1;  IOCON3bits.PMOD = 3;
                        IOCON3bits.PENH = 1, IOCON3bits.POLH = 1; break;
        case PWM3_L:    PWMCON3bits.ITB = 1;  IOCON3bits.PMOD = 3;
                        IOCON3bits.PENH = 1, IOCON3bits.POLH = 0; break;
        
        case PWM4_H:    PWMCON4bits.ITB = 1;  IOCON4bits.PMOD = 3;
                        IOCON4bits.PENH = 1, IOCON4bits.POLH = 1; break;
        case PWM4_L:    PWMCON4bits.ITB = 1;  IOCON4bits.PMOD = 3;
                        IOCON4bits.PENH = 1, IOCON4bits.POLH = 0; break;
        
        case PWM5_H:    PWMCON5bits.ITB = 1;  IOCON5bits.PMOD = 3;
                        IOCON5bits.PENH = 1, IOCON5bits.POLH = 1; break;
        case PWM5_L:    PWMCON5bits.ITB = 1;  IOCON5bits.PMOD = 3;
                        IOCON5bits.PENH = 1, IOCON5bits.POLH = 0; break;
        
        case PWM6_H:    PWMCON6bits.ITB = 1;  IOCON6bits.PMOD = 3;
                        IOCON6bits.PENH = 1, IOCON6bits.POLH = 1; break;
        case PWM6_L:    PWMCON6bits.ITB = 1;  IOCON6bits.PMOD = 3;
                        IOCON6bits.PENH = 1, IOCON6bits.POLH = 0; break;
         
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
        case PWM1_H: PHASE1 = PR; break;
        case PWM1_L: SPHASE1 = PR; break;
        
        case PWM2_H: PHASE2 = PR; break;
        case PWM2_L: SPHASE2 = PR; break; 
        
        case PWM3_H: PHASE3 = PR; break;
        case PWM3_L: SPHASE3 = PR; break; 
        
        case PWM4_H: PHASE4 = PR; break;
        case PWM4_L: SPHASE4 = PR; break; 
        
        case PWM5_H: PHASE5 = PR; break;
        case PWM5_L: SPHASE5 = PR; break; 
        
        case PWM6_H: PHASE6 = PR; break;
        case PWM6_L: SPHASE6 = PR; break;   
    }
}




void setPwmDuty(pwm* p, double percent){
    
    uint16_t DC = (p->period)/(100.0/percent);

    switch (p -> n){
        case PWM1_H: PDC1 = DC; break;
        case PWM1_L: SDC1 = DC; break;
        
        case PWM2_H: PDC2 = DC; break;
        case PWM2_L: SDC2 = DC; break;
        
        case PWM3_H: PDC3 = DC; break;
        case PWM3_L: SDC3 = DC; break;
        
        case PWM4_H: PDC4 = DC; break;
        case PWM4_L: SDC4 = DC; break;
        
        case PWM5_H: PDC5 = DC; break;
        case PWM5_L: SDC5 = DC; break;
        
        case PWM6_H: PDC6 = DC; break;
        case PWM6_L: SDC6 = DC; break;
    }
}



void setPwmDutyLimits(pwm *p, int min_time,int max_time){
    p->min_time = min_time;
    p->range_time = max_time - min_time;
}

void setPwmDutyTime(pwm *p, double percent){
    double time = (p->range_time)*percent/100.0 + p -> min_time;
    uint16_t DC = (time) * ( FCY/(1000000LL * master_prescaler) )*2LL   -  1;


    switch (p -> n){
        case PWM1_H: PDC1 = DC; break;
        case PWM1_L: SDC1 = DC; break;
        
        case PWM2_H: PDC2 = DC; break;
        case PWM2_L: SDC2 = DC; break;
        
        case PWM3_H: PDC3 = DC; break;
        case PWM3_L: SDC3 = DC; break;
        
        case PWM4_H: PDC4 = DC; break;
        case PWM4_L: SDC4 = DC; break;
        
        case PWM5_H: PDC5 = DC; break;
        case PWM5_L: SDC5 = DC; break;
        
        case PWM6_H: PDC6 = DC; break;
        case PWM6_L: SDC6 = DC; break;
    }
}
