#ifndef SPD610_500Pa_H
#define SPD610_500Pa_H


#include "i2c.h"

typedef struct _SDP610_500{
    i2c com;
    int n;
    double val;    
} SDP610_500;


void initSDP610_500(SDP610_500* f, int n);
int readSDP610_500(SDP610_500* f);

#endif