#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string.h>

void int32pack(unsigned char *bytesint32,int32_t &n);
void int32unpack(int32_t n,unsigned char *bytesint32);
void floatunpack(float n,unsigned char *bytesfloat);
void floatpack(unsigned char *bytesfloat,float &n);
void print4bytes(unsigned char *data);

#endif