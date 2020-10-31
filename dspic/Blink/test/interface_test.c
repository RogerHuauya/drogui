#define INTERFACE_TEST
#ifdef INTERFACE_TEST
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"
#include <xc.h>
#include "timer.h"
#include "control.h"
#include "MM7150.h"
#include "pwm.h"
#include "utils.h"
#include "registerMap.h"

int press = 0;
char crc = 0;
//int s1, s2, s3;


char c1,c2,c3;
i2c slave;
serial Serial1;
int main(){
    char s[50];

    initConfig();
    initSerial(&Serial1, SERIAL1,  115200);
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    __delay_ms(1000);
    serialWriteString(&Serial1, "Hola mundo\n");
    while(1){
    /*    
        serialWriteString(&Serial1, "Hola Roger\n");
        sprintf(s, "Bytes:%d %d %d %d \n",\
        (int)i2c2Reg[M1_VAL], (int)i2c2Reg[M1_VAL + 1], (int)i2c2Reg[M1_VAL +2], (int)i2c2Reg[M1_VAL + 3]);
        serialWriteString(&Serial1, s);
        setReg(M1_VAL, 1.259);
        sprintf(s, "Bytes:%d %d %d %d \n",\
        (int)i2c2Reg[M1_VAL], (int)i2c2Reg[M1_VAL + 1], (int)i2c2Reg[M1_VAL +2], (int)i2c2Reg[M1_VAL + 3]);
        serialWriteString(&Serial1, s);
        float aux = getReg(M1_VAL);
        sprintf(s, "%.3f \n" , aux);
        serialWriteString(&Serial1, s);
        __delay_ms(10000);
    */
    float aux = getReg(M1_VAL);
    sprintf(s, "Regiter M1 VAL: %.3f\n", aux);
    serialWriteString(&Serial1, s);
    __delay_ms(1000);
    }
    return 0;
}
#endif