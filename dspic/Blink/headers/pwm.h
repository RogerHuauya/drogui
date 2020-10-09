#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include "config.h"
#include <stdbool.h>
#include <stdint.h>

enum PWM_PINS { PWM1_H, PWM1_L, PWM2_H, PWM2_L, PWM3_H, PWM3_L,\
                PWM4_H, PWM4_L, PWM5_H, PWM5_L, PWM6_H, PWM6_L};

typedef struct _pwm{
    int n;
    uint16_t period; 
    uint16_t min_time, range_time;
} pwm;


void initPwmPin(pwm* p, int n);
void initPwm();
void setPwmPrescaler(int prescaler);
void setPwmFrecuency(pwm *p, double freq);

void setPwmDuty(pwm* p, double percent);

void setPwmDutyLimits(pwm *p, int min_time,int max_time);
void setPwmDutyTime(pwm *p, double percent);


#endif