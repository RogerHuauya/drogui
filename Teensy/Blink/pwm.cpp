#include "pwm.h"

short init_done = 0;
int master_prescaler = 0;

void initPwmPin(pwm* p, int n){
    p->n = n;
    analogWriteResolution(16);
    pinMode(n, OUTPUT);
}



void setPwmFrecuency(pwm* p, double freq){
    analogWriteFrequency(p->n,3500);
    p->period = 1000000.0/freq;
}




void setPwmDuty(pwm* p, double percent){
    analogWrite(p->n, ((1 << 16)-1)*(percent/100)  );
}

void setPwmDutyLimits(pwm *p, int min_time,int max_time){
    p->min_time = min_time;
    p->range_time = max_time - min_time;
}

void setPwmDutyTime(pwm *p, double percent){
    double time = (p->range_time)*percent/100.0 + p -> min_time;
    analogWrite(p->n, time/(p->period) * ((1 << 16)-1));

}

void initOneshot125(pwm* p, int pin){
    initPwmPin(p, pin);
    setPwmDutyLimits(p, 125, 250);
    setPwmFrecuency(p, 3500);
    setPwmDutyTime(p, 0);
}
