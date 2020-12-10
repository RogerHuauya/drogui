#include "SMI_9541.h"
#include "serial.h"

#define ADD 0x28
#define FREQ 200000

void initSMI_9541(SMI_9541 * p, int n){
    initI2C(&(p->com), n, ADD, FREQ);
}


int readSMI_9541(SMI_9541* p){


    short press = 0, temp = 0;
    i2cRestart(&(p->com));
    i2cStartRead(&(p->com));
    press  = ((i2cRead(&(p->com)) & 0x3F) << 8); i2cSendACK(&(p->com));
    press |= (i2cRead(&(p->com))); i2cSendACK(&(p->com));
    temp = (i2cRead(&(p->com))<< 3); i2cSendACK(&(p->com));
    temp |= (i2cRead(&(p->com)) >> 5) & (7); i2cSendNACK(&(p->com));
    
    p -> val = 105.0*(press-1638.0)/(13107.0) - 5.0;
    
    return 0;
}
