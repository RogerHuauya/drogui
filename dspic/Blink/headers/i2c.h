#ifndef I2C_H
#define I2C_H
#include <xc.h>
#include <stdbool.h>

void initI2C();
int i2cWrite(unsigned char data);
void i2cStart();
void i2cStop();
void i2cRestart();
int i2cWriteString(unsigned char* s);
unsigned char i2cRead();
int i2cReadString(unsigned char *s, int len);

void i2cSendACK();
void i2cSendNACK();

char i2cAvailable();

#endif