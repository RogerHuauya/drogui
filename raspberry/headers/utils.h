#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string.h>

int32_t bytestoint32(uint8_t *bytesint32);
void int32tobytes(int32_t n,uint8_t *bytesint32);
void floattobytes(float n, uint8_t* bytesfloat);
float bytestofloat(uint8_t *bytesfloat);
void print4bytes(uint8_t *data);

#endif