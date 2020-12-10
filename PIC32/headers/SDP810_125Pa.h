#ifndef SPD810_125Pa_H
#define SPD810_125Pa_H


#include "i2c.h"
#include "utils.h" 

typedef struct _SDP810_125{
    i2c com;
    int n;
    double val;    
} SDP810_125;


void initSDP810_125(SDP810_125* f, int n);
int readSDP810_125(SDP810_125* f);

#endif