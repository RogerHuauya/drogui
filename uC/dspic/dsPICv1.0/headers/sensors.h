#ifndef SENSORS_H
#define SENSORS_H

#include <xc.h>
#include "filter.h"
#include "utils.h"
#include "analog.h"



#define SPAN_P 93.46425351445826
#define SPAN_Q 9346.425351445826
#define ADC_RES 1023.0

typedef struct _sensorP{
    int pin;
    double offset, val;
    filter* f;
} sensorP;

typedef struct _sensorQ{
    int pin;
    double offset, val, C;
    double d2, d1, rho;
    filter* f;
} sensorQ;
void initSensorP(sensorP * sP, int pin, double offset);
void readSensorP(sensorP * sP);
void initSensorQ(sensorQ * sQ, int pin, double offset);
void readSensorQ(sensorQ* sQ);

#endif