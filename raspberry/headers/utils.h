<<<<<<< HEAD
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string.h>

#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)    "\033[1;37m"#n"\033[0m"

void int32pack(uint8_t *bytesint32,int32_t *n);
void int32unpack(int32_t n,uint8_t *bytesint32);
void floatunpack(float n,uint8_t *bytesfloat);
void floatpack(uint8_t *bytesfloat,float *n);
void print4bytes(uint8_t *data);

=======
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string.h>


#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)    "\033[1;37m"#n"\033[0m"

int32_t bytestoint32(uint8_t *bytesint32);
void int32tobytes(int32_t n,uint8_t *bytesint32);
void floattobytes(float n, uint8_t* bytesfloat);
float bytestofloat(uint8_t *bytesfloat);
void print4bytes(uint8_t *data);

>>>>>>> roger
#endif