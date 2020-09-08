#ifndef I2C_H
#define I2C_H
#include <xc.h>

enum I2C_PORTS {I2C1, I2C2, I2C4};

typedef struct _i2c{
    int n; char address;
} i2c;

void idleI2C(i2c *c);
void initI2C(i2c* c, int n, char adress, double freq);
int i2cWrite(i2c* c, unsigned char data);
void i2cStart(i2c* c);
void i2cStop(i2c* c);
void i2cRestart(i2c* c);
int i2cWriteString(i2c* c , char* s);
unsigned char i2cRead(i2c* c);
int i2cReadString(i2c* c,char *s, int len);

void i2cSendACK(i2c* c);
void i2cSendNACK(i2c* c);

char i2cAvailable(i2c* c);


int i2cStartRead(i2c* c);
int i2cStartWrite(i2c* c);

#endif