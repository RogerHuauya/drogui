#ifndef I2C_H
#define I2C_H

#include <Arduino.h>

#define TIME_OUT 100000

enum I2C_PORTS {I2C1, I2C2, I2C4};
enum I2C_MODE {MASTER, SLAVE};
enum I2C_ERR {SUCCESS, IDLE_ERROR, ACK_ERROR, COL_ERROR, TRM_ERROR, \
                    RCV_ERROR, TIMEOUT_ERROR, TMR_FULL_ERROR, RBF_ERROR};

typedef struct _i2c{
    int n; uint8_t address;
    uint8_t mode;
} i2c;


#define I2C_REG_SIZE 300
extern uint8_t i2c1Reg[I2C_REG_SIZE];
extern uint8_t i2c2Reg[I2C_REG_SIZE];

void clearI2Cregisters(int n);
void initI2C(int mode,int n, int adress);
    

#endif
