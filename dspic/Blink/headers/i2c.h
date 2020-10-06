#ifndef I2C_H
#define I2C_H
#include <xc.h>
#include "config.h"
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

enum I2C_PORTS {I2C1, I2C2, I2C4};
enum I2C_MODE {MASTER, SLAVE};
typedef struct _i2c{
    int n; uint8_t address;
    uint8_t mode;
    uint8_t reg[20];
} i2c;

extern uint8_t i2c1Reg[20];

void idleI2C(i2c *c);
void initI2C(i2c* c, int n, uint8_t address, double freq, int mode);
void i2cInitRegister(i2c* c);

int i2cWrite(i2c* c, uint8_t data);
void i2cStart(i2c* c);
void i2cStop(i2c* c);
void i2cRestart(i2c* c);
int i2cWriteString(i2c* c, uint8_t* s);
unsigned char i2cRead(i2c* c);
int i2cReadString(i2c* c, uint8_t *s, int len);

void i2cSendACK(i2c* c);
void i2cSendNACK(i2c* c);

char i2cAvailable(i2c* c);


int i2cStartRead(i2c* c);
int i2cStartWrite(i2c* c);

#endif