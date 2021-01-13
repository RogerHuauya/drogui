#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

#include "..\headers\i2c.h"

uint8_t i2c1Reg[I2C_REG_SIZE];
uint8_t i2c2Reg[I2C_REG_SIZE];
int ptr[20], ptr_i = 0, ptr_j = 0;
//int ptr;

void clearI2Cregisters(int n){
    if(n == I2C1) for(int i = 0; i < I2C_REG_SIZE; i++) i2c1Reg[i] = 0;     
    else if(n == I2C2) for(int i = 0; i < I2C_REG_SIZE; i++) i2c2Reg[i] = 0;
     
}

void requestI2C1(){
    Wired1.write(i2c1Reg[ptr_j++]);
    ptr_j %= 20;  
}

void receiveI2C1(int reg){
  
    int x = Wired1.read();
    if(Wired1.available()){
      i2c1Reg[x] = Wired1.read();
      Serial.print("WReg: ");
      Serial.println(reg);
    }
    else{
        ptr[ptr_i++] = x, ptr_i %= 20;
        Serial.print("Reg: ");
        Serial.println(reg);
    }
}

void initI2C(int mode,int n, int adress){
    if(n == I2C1){
        Wired1.begin(adress);
        Wired1.onRequest(requestI2C1);
        Wired1.onReceive(receiveI2C1); 
    }
}
