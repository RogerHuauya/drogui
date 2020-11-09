#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <string.h>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <unistd.h> 

#include <iomanip>
#include <ctime>
#include <sstream>
//#include <stdlib.h>
//#include <stdio.h>


#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)    "\033[1;37m"#n"\033[0m"


class rasp_I2C{
    int fd;
    public:
        rasp_I2C(int ADDRESS);
        int32_t bytestoint32(uint8_t *bytesint32);
        void int32tobytes(int32_t n,uint8_t *bytesint32);
        void floattobytes(float n, uint8_t* bytesfloat);
        float bytestofloat(uint8_t *bytesfloat);
        void sendFloat(uint8_t reg, float val);
        float readFloat(uint8_t reg);
        void print4bytes(uint8_t *data);
        void writeMCU(uint8_t reg, uint8_t val);
        uint8_t readMCU(uint8_t reg);
};

void cls();
std::string str_datetime();

#endif