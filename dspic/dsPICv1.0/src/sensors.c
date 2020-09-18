#include "sensors.h"

void initSensorP(sensorP * sP, int pin, double offset){
    sP->pin = pin; sP->offset = offset; 
    sP->f = malloc(sizeof(filter));
    initFilter(sP->f, 0.8, 1, 1);
}


void readSensorP(sensorP * sP){ 
    sP->val = analogRead(sP->pin)*(SPAN_P)/ADC_RES + sP->offset;
    sP->val = computeFilter(sP->f, sP->val);
}

void initSensorQ(sensorQ * sQ, int pin, double offset){    
    sQ -> d2 = (3.29)*1e-3;
    sQ -> d1 = (12.4)*1e-3;
    sQ -> rho = 1.225;
    sQ -> pin = pin; sQ->offset = offset;
    sQ->f = malloc(sizeof(filter));
    initFilter(sQ->f, 0.8, 1, 1);
}
    
void readSensorQ(sensorQ* sQ){
    //double val = sQ -> val; 
    sQ->val = analogRead(sQ->pin)*(SPAN_Q)/ADC_RES + sQ->offset;
    if(sQ->val == sQ->offset) sQ->val = 0;

    sQ -> val = sqrt(sQ -> val*pi*pi/(8*sQ -> rho*(1/pow(sQ -> d2,4)- 1/pow(sQ -> d1,4))));
    sQ -> val *= 60000;
    
    sQ -> val = computeFilter(sQ -> f, sQ -> val);
}
