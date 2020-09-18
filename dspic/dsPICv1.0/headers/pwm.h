#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include "config.h"

typedef struct _pwm{
    int mod, n, prescaler, postscaler, period;
} pwm;


void initPwm(pwm* p, int mod, int n, int prescaler, int postscaler);
void setPwmFrecuency(pwm* p, int freq);
void setPwmDuty(pwm* p, double percent);

#endif