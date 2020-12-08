#ifndef INT_H
#define INT_H
#include <xc.h>

#define intInterrupt(n, p) __attribute__((vector(_EXTERNAL_##n##_VECTOR), interrupt(ipl##p##auto), nomips16)) _Int##n##Interrupt(void)

void initInterrupts(int n, int priority);
void clearIntFlag(int n, int priority);

#endif