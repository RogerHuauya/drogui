#ifndef PWM_H
#define PWM_H


#include <Arduino.h>
typedef struct _pwm{
    int n;
    float period; 
    float min_time, range_time;
} pwm;


void initPwmPin(pwm* p, int n);
void setPwmFrecuency(pwm *p, float freq);
void setPwmDuty(pwm* p, float percent);
void setPwmDutyLimits(pwm *p, int min_time,int max_time);
void setPwmDutyTime(pwm *p, float percent);
void initOneshot125(pwm *p, int pin);


#endif
