#include "SDP810_125Pa.h"

#define ADD 0x25
#define CMD_MSB 0x36
#define CMD_LSB 0x1E
#define FREQ 200000

void initSDP810_125(SDP810_125* f, int n){
    initI2C(&(f->com), n, ADD, FREQ);
    delayMs(10);
    i2cStart(&(f->com));
    i2cStartWrite(&(f->com));
    i2cWrite(&(f->com), CMD_MSB);
    i2cWrite(&(f->com), CMD_LSB);
}


int readSDP810_125(SDP810_125* f){
    
    short press = 0; 
    unsigned char crc;
                
    i2cRestart(&(f->com));
    i2cStartRead(&(f->com));
    
    press = i2cRead(&(f->com)); i2cSendACK(&(f->com));
    press = (press << 8) | i2cRead(&(f->com)); i2cSendACK(&(f->com));
    crc = i2cRead(&(f->com)); i2cSendNACK(&(f->com));
    
    f->val = press/240.0;

    return 0;
}