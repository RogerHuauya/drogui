#ifndef UTILS_H
#define UTILS_H

enum commands {CONFIG_P, CONFIG_S, START, STOP, STANBY, RESET, RESEND};
enum modes {CMV_V, CMV_P, ACMV_V, ACMV_P, SIMV_V, SIMV_P, CPAP, PSV};
enum parameters {RR, PRS, FIO2, PEEP, TI, VT, SLP, TRG_F, TRG_P, MOD}; 




#include <math.h>
#include <stdlib.h>
#include "io.h"
#include <xc.h>

#define abs(x) ( (x > 0) ? (x) : ((-1)*(x)))
#define pi acos(-1)
#define EVAL(func, a) func(a)
#define LED1 PRTA, 10
#define LED2 PRTB, 11

#define BOB_INS PRTG, 9
#define BOB_ESP PRTE, 8
#define BOB_FIL PRTB, 7
#define eps 1e-6

void delayUs(unsigned int us);
void delayMs(unsigned int ms);
void startDelay();
void endDelayUs(unsigned int us);
void endDelayMs(unsigned int ms);

#endif
