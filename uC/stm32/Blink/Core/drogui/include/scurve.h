#ifndef SCURVE_H
#define SCURVE_H
#include "_main.h"

typedef struct _scurve{
    
    float ini, fin, period;
    uint32_t ini_time;

} scurve;

float t2s(scurve *sc, float time);
void setTrayectory(scurve *sc, float ini, float fin, float period, uint32_t ini_time);
float getSetpoint(scurve *sc, uint32_t time);



#endif