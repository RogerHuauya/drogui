#ifndef I2C_H
#define I2C_H
#include <xc.h>
#include "config.h"
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

#define TIME_OUT 1000

enum I2C_PORTS {I2C1, I2C2, I2C4};
enum I2C_MODE {MASTER, SLAVE};
enum I2C_ERR {SUCCESS, IDLE_ERROR, ACK_ERROR, COL_ERROR, TRM_ERROR, \
                    RCV_ERROR, TIMEOUT_ERROR, TMR_FULL_ERROR, RBF_ERROR};

typedef struct _i2c{
    int n; uint8_t address;
    uint8_t mode;
} i2c;

extern uint8_t i2c1Reg[20];
extern uint8_t i2c2Reg[20];

int idleI2C(i2c *c);
void initI2C(i2c* c, int n, uint8_t address, double freq, int mode);
int i2cInitRegister(i2c* c);
void rebootI2C(i2c *c);
int i2cWrite(i2c* c, uint8_t data);
int i2cStart(i2c* c);
int i2cStop(i2c* c);
int i2cRestart(i2c* c);
int i2cWriteString(i2c* c, uint8_t* s);
int i2cRead(i2c* c, uint8_t* data);
int i2cReadString(i2c* c, uint8_t *s, int len);
int i2cSendACK(i2c* c);
int i2cSendNACK(i2c* c);
int i2cStartRead(i2c* c);
int i2cStartWrite(i2c* c);

#endif