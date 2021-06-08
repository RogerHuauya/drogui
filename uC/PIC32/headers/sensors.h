#ifndef SENSORS_H
#define SENSORS_H

#include <xc.h>
#include "utils.h"
#include "SDP610_500Pa.h"
#include "SDP810_125Pa.h"
#include "SMI_9541.h"


typedef struct _press{
    SMI_9541 sensor;
    double val;
} press;

typedef struct _flow{
    SDP610_500 sensor;
    double val;
} flow;


void initPress(press * p, int n);
void readPress(press * p);
void initFlow(flow * f, int n);
void readFlow(flow * f);

#endif