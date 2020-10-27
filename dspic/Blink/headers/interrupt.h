#ifndef INT_H
#define INT_H
#include <xc.h>

#define intInterrupt(n) __attribute__((__interrupt__, no_auto_psv)) _INT##n##Interrupt(void)

void initInterrupt(int n, int priority);
void clearIntFlag(int n);

#endif