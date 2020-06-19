#include "i2c.h"



void idleI2C(){
    while(I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN 
            || I2CCONbits.ACKEN || I2CSTATbits.TRSTAT);	
}

void initI2C(){
    
    I2CCONbits.A10M = 0;       // 7bit address
    I2CCONbits.I2CSIDL = 0;
    I2CCONbits.I2CEN = 1;    

    I2CBRG = 20;//((1/FSCL - Delay)*FCY/2) - 2;
}

void i2cStart(){
    I2CCONbits.SEN = 1;
}

void i2cStop(){
     I2CCONbits.PEN = 1;
}

void i2cRestart(){
    I2CCONbits.RSEN = 1;
}

int i2cWrite(char data){
    I2CTRN = data;
    if(I2CSTATbits.IWCOL)        
        return -1;
    else{

        idleI2C();                  
        if( I2CSTATbits.ACKSTAT ) 
    	    return -2;
	    else return 0;

    }
}

int i2cWriteString(char * s){
    while(*s){
        if(i2cWrite(*s) == -1)
            return -3;                         
        while(I2CSTATbits.TBF);
        idleI2C();
        s++;
    }
    return 0;			
}


char i2cRead(){
    I2CCONbits.RCEN = 1;
    while(I2CCONbits.RCEN);
    I2CSTATbits.I2COV = 0;
    return I2C1RCV;
}

int i2cReadString(char* s, int len){
    while(len){
        I2CCONbits.RCEN = 1;
        while(!i2cAvailable());
        
        *s = I2CRCV;
        s++;  len--;
        if(len == 0)
            I2CCONbits.ACKDT = 1; // nack
        else
            I2CCONbits.ACKDT = 0; // ack

        I2CCONbits.ACKEN = 1;
        while(I2CCONbits.ACKEN == 1);
    }
}


void i2cSendACK(){
    I2CCONbits.ACKDT = 0;
    I2CCONbits.ACKEN = 1;
}

void i2cSendNACK(){
    I2CCONbits.ACKDT = 1;
    I2CCONbits.ACKEN = 1;
}

char i2cAvailable(){
    return I2CSTATbits.RBF;
}

