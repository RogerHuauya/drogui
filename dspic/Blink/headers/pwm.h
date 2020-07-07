#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include "config.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct _pwm{
    int n;
    uint16_t period; 
    bool primary;
} pwm;


void initPwmPin(pwm* p, int n, bool primary);
void initPwm();
void setPwmPrescaler(int prescaler);
void setPwmFrecuency(pwm *p, double freq);

void setPwmDuty(pwm* p, double percent);


#endif