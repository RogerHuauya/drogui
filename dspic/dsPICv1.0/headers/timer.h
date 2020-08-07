#ifndef TIMER_H
#define TIMER_H

//define ISR(n) __attribute__((__interrupt__, no_auto_psv)) _T##n##Interrupt(void)
#include <xc.h>
#include "config.h"

typedef struct _timer{
    int n;
    int prescaler;
} timer;

void initTimer(timer*, int, int, int);
void clearFlag(timer*);

#endif
