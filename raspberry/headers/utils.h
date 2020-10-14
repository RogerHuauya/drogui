#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string.h>

void int32pack(uint8_t *bytesint32,int32_t *n);
void int32unpack(int32_t n,uint8_t *bytesint32);
void floatunpack(float n,uint8_t *bytesfloat);
void floatpack(uint8_t *bytesfloat,float *n);
void print4bytes(uint8_t *data);

#endif