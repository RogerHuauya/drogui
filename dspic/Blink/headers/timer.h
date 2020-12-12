#ifndef TIMER_H
#define TIMER_H

#define timerInterrupt(n) __attribute__((__interrupt__, no_auto_psv)) _T##n##Interrupt(void)
#include <xc.h>
#include "config.h"

enum divs {DIV1, DIV8, DIV64, DIV256};


typedef struct _timer{
    int n, prescaler;
} timer;

void initTimer(timer*, int n, int pre, int priority);
void clearTimerFlag(timer*);
void setTimerFrecuency(timer*, unsigned long long freq);

#endif
