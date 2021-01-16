#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

#include "..\headers\i2c.h"

uint8_t i2c1Reg[I2C_REG_SIZE][4];

#define I2C_BUFF_SIZE 200
//int ptr[I2C_BUFF_SIZE], ptr_head = 0, ptr_tail = 0;
int ptr,ptr_head = 0, ptr_tail = 0;
//int ptr;

void clearI2Cregisters(int n){
    if(n == I2C1) 
        for(int i = 0; i < I2C_REG_SIZE; i++) 
            for(int j = 0; j < 4 ; j++)
                i2c1Reg[i][j] = 0;     
    //else if(n == I2C2) for(int i = 0; i < I2C_REG_SIZE; i++) i2c2Reg[i] = 0;
     
}

void requestI2C1(){
    //int x = ptr[ptr_tail++];
    //ptr_tail %= I2C_BUFF_SIZE;
    int x = ptr;
    /*Serial.print("Requested x: ");
    Serial.println(x);*/
	for(int i = 0; i < 4 ; i++)	Wired1.write(i2c1Reg[x][i]);
}

void receiveI2C1(int reg){
  
    int x = Wired1.read();
    //Serial.print("Receive x: ");
    //Serial.println(x);
    if(x & 1){
        //Serial.println("Receive Only x: ");
    	for(int i = 0; i < 4 ; i++) {
            if(!Wired1.available()) Serial.println("No disponible");
            i2c1Reg[x-1][i] = Wired1.read();
        }
    }
    else{
        //Serial.println("Is writing: ");
        ptr = x;
    }
    //Serial.println("");
}

void initI2C(int mode,int n, int adress){
    if(n == I2C1){
        Wired1.begin(adress);
        //Serial.println("Init ");
        Wired1.onRequest(requestI2C1);
        Wired1.onReceive(receiveI2C1); 
    }
}
