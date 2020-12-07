#include "SDP610_500Pa.h"

#define ADD 64
#define FREQ 200000
#define READ_CMD 0xF1

void initSDP610_500(SDP610_500* f, int n){
    initI2C(&(f->com), n, ADD, FREQ);
}


int readSDP610_500(SDP610_500* f){

    short press;
    unsigned char crc;
    i2cStart(&(f->com));
    i2cStartWrite(&(f->com));
    i2cWrite(&(f->com), READ_CMD);
    
    i2cStart(&(f->com));
    i2cStartRead(&(f->com));

    press  = (i2cRead(&(f->com))<<8); i2cSendACK(&(f->com));
    press |= (i2cRead(&(f->com))); i2cSendACK(&(f->com));
    crc = (i2cRead(&(f->com))); i2cSendACK(&(f->com));
    
    i2cStop(&(f->com));
    
    f->val = press/60.0;
    return 0;
}
  
