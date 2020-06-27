#ifndef CONFIG_H
#define CONFIG_H
#include <xc.h>


#define FCY 40000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1

void initConfig();
#endif	/* XC_HEADER_TEMPLATE_H */

