#ifndef CONFIG_H
#define CONFIG_H
#include <xc.h>

#define FCY 40000000LL
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/4)-1

void initConfig();

#endif	

