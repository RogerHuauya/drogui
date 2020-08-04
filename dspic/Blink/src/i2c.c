#include "i2c.h"
#include "serial.h"

void idleI2C(){
    while(I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RCEN 
            || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT);	
}

void initI2C(){
    
    I2C2CONbits.A10M = 0;       // 7bit address
    I2C2CONbits.I2CSIDL = 0;
    I2C2BRG = 0x23; 
    I2C2CONbits.I2CEN = 1;    
    i2cStop();
    //((1/FSCL - Delay)*FCY/2) - 2;
}

void i2cStart(){
    idleI2C();
    I2C2CONbits.SEN = 1;
}

void i2cStop(){
    idleI2C();
     I2C2CONbits.PEN = 1;
}

void i2cRestart(){
    idleI2C();
    I2C2CONbits.RSEN = 1;
}

int i2cWrite(unsigned char data){
    idleI2C();
    while(I2C2STATbits.TBF){}
    I2C2TRN = data;
    if(I2C2STATbits.IWCOL)        
        return -1;
    else{
        idleI2C();                  
        if( I2C2STATbits.ACKSTAT ) 
    	    return -2;
	    else return 0;
    }
}   

int i2cWriteString(unsigned char * s){
    while(*s){
        if(i2cWrite(*s) == -1)
            return -3;                         
        while(I2C2STATbits.TBF);
        idleI2C();
        s++;
    }
    return 0;
}


unsigned char i2cRead(){
    idleI2C();
    I2C2CONbits.RCEN = 1;
    while(I2C2CONbits.RCEN){};
    I2C2STATbits.I2COV = 0;
    return I2C2RCV;
}

int i2cReadString(unsigned char* s, int len){
    while(len){
        I2C2CONbits.RCEN = 1;
        while(!i2cAvailable());
        
        *s = I2C2RCV;
        s++;  len--;
        if(len == 0)
            I2C2CONbits.ACKDT = 1; // nack
        else
            I2C2CONbits.ACKDT = 0; // ack

        I2C2CONbits.ACKEN = 1;
        while(I2C2CONbits.ACKEN == 1);
    }
    return 0;
}


void i2cSendACK(){
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
}

void i2cSendNACK(){
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
}

char i2cAvailable(){
    return I2C2STATbits.RBF;
}
