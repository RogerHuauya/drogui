#ifndef SMI_9541_H
#define SMI_9541_H

#include "i2c.h"


typedef struct _SMI_9541{
    i2c com;
    int n;
    double val;
} SMI_9541;

void initSMI_9541(SMI_9541* p, int n);
int readSMI_9541(SMI_9541 *p);

#endif