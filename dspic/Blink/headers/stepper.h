#ifndef STEPPER_H    
#define STEPPER_H
#include <stdbool.h>
#include <xc.h>
#include "timer.h"

#define PIN_DIR PORTBbits.RB10
#define PIN_STP PORTBbits.RB11
#define STP_SIZE 1.8


void initStepper();
void step();
void setDir(bool);
void setVelStepper(double vel);
void stopStepper();










#endif