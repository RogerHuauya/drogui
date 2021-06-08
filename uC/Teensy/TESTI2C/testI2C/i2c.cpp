#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

#include "i2c.h"

uint8_t i2c1Reg[I2C_REG_SIZE];
uint8_t i2c2Reg[I2C_REG_SIZE];

int ptr;

void clearI2Cregisters(int n){
    if(n == I2C1) for(int i = 0; i < I2C_REG_SIZE; i++) i2c1Reg[i] = 0;     
    else if(n == I2C2) for(int i = 0; i < I2C_REG_SIZE; i++) i2c2Reg[i] = 0;
     
}

void requestI2C1(){
  Serial.println("request");
  Wired1.write(i2c1Reg[ptr]);  
  Serial.println(i2c1Reg[ptr]);
}

void receiveI2C1(int reg){
  
    ptr = Wired1.read();
    if(Wired1.available()){
      i2c1Reg[ptr] = Wired1.read();
      Serial.println("receive Write");
      Serial.print(ptr);
      Serial.print(" ");
      Serial.println(i2c1Reg[ptr]);
    }
    else{
      Serial.println("receive Read");
      Serial.println(ptr);
      }
}



void initI2C(int mode,int n, int adress){
    if(n == I2C1){
        Wired1.begin(adress);
        Wired1.onRequest(requestI2C1);
        Wired1.onReceive(receiveI2C1); 
    }
}
