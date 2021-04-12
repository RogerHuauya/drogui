#include "scurve.h"


float t2s(scurve *sc, float time){
    float tT = time / (sc -> period);
    return 10*tT*tT*tT - 15*tT*tT*tT*tT + 6*tT*tT*tT*tT*tT;
}

void setTrayectory(scurve *sc, float ini, float fin, float period, uint32_t ini_time){
    sc->ini = ini;
    sc->fin = fin;
    sc->period = 1000000 * period;
    sc->ini_time = ini_time;
}

float getSetpoint(scurve *sc, uint32_t time){

    if(time - sc->ini_time > sc->period) 
        return sc->fin;

    float s = t2s(sc, time - sc->ini_time);
    return (sc->ini) + s*(sc->fin - sc->ini);
}

