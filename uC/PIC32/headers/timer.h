#ifndef TIMER_H
#define TIMER_H

#define timerInterrupt(n, pr) __attribute__((vector(_TIMER_##n##_VECTOR), interrupt(ipl##pr##auto), nomips16)) _T##n##Interrupt(void)
#include <xc.h>
#include "config.h"

enum divs {DIV1, DIV2, DIV4, DIV8, DIV16, DIV32, DIV64, DIV256};


typedef struct _timer{
    int n, prescaler;
} timer;

void initTimer(timer*, int n, int priority, int pre);
void clearTimerFlag(timer*);
void setTimerFrecuency(timer*, double freq);

#endif
