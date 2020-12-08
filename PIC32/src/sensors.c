#include "sensors.h"

void initPress(press * p, int n){
    initSMI_9541(&(p -> sensor), n);
}

void readPress(press * p){ 
    readSMI_9541(&(p->sensor));
    p -> val = (p -> sensor).val;
}

void initFlow(flow * f, int n){    
    initSDP610_500(&(f -> sensor), n);
}
    
void readFlow(flow* f){
    readSDP610_500(&(f -> sensor));
    
    double x = (f -> sensor).val;
    int sign = (x > 0 ? 1 : -1);
    f -> val = -0.0473*x*x*sign + 3.108 * x; 
}
