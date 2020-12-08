#ifndef PEEP_H
#define PEEP_H

#include "utils.h"
#include "stepper.h"
#include "sensors.h"

#define epsPeep 1

typedef struct _peep{
    double value;
    stepper motor;
} peep;


void initPeep(peep* pp, int dir_port, int dir_pin, int stp_port, int stp_pin);
void setPeepPress(peep* pp, press* p);
void stepPeep(peep * pr, bool dir);

void readPeep(peep* pp);
double peep2Pos(double pp);
double getPeepPos(peep* p);



#endif