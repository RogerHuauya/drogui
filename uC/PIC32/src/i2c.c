#include "i2c.h"
#include "serial.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

char buff[50];
bool on1 = false, on2 = false , on3 = false;

void idleI2C(i2c *c){
    switch(c -> n){
        case I2C1: while(I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || I2C1CONbits.RSEN
                        || I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT);	break;
        case I2C2: while(I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RCEN || I2C2CONbits.RSEN
                        || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT);   break;
        case I2C4: while(I2C4CONbits.SEN || I2C4CONbits.PEN || I2C4CONbits.RCEN || I2C4CONbits.RSEN
                        || I2C4CONbits.ACKEN || I2C4STATbits.TRSTAT);	break;
    }
}

void initI2C(i2c* c, int n, char adress, double freq){
    c -> n = n;
    c -> address = adress;

    int BRG = ((1 / (2 * freq)) - 0.000000104) * ((SYS_FREQ / 2) - 2);
    switch(c->n){
        
        case I2C1: if(on1) break; I2C1BRG = BRG; I2C1CONbits.DISSLW = 0; I2C1CONbits.ON = 1; on1 = 1; break;
        case I2C2: if(on2) break; I2C2BRG = BRG; I2C2CONbits.DISSLW = 0; I2C2CONbits.ON = 1; on2 = 1; break;
        case I2C4: if(on3) break; I2C4BRG = BRG; I2C4CONbits.DISSLW = 0; I2C4CONbits.ON = 1; on3 = 1;  break;
    } 
}

void i2cStart(i2c* c){
    idleI2C(c);
    switch(c -> n){
        case I2C1: I2C1CONbits.SEN = 1; break;
        case I2C2: I2C2CONbits.SEN = 1; break;
        case I2C4: I2C4CONbits.SEN = 1; break;
    }
}

void i2cStop(i2c* c){
    idleI2C(c);
    switch(c -> n){
        case I2C1: I2C1CONbits.PEN = 1; break;
        case I2C2: I2C2CONbits.PEN = 1; break;
        case I2C4: I2C4CONbits.PEN = 1; break;
    }
}

void i2cRestart(i2c* c){
    idleI2C(c);
    switch(c -> n){
        case I2C1: I2C1CONbits.RSEN = 1; break;
        case I2C2: I2C2CONbits.RSEN = 1; break;
        case I2C4: I2C4CONbits.RSEN = 1; break;
    }
}

int i2cWrite(i2c* c, char data){
    idleI2C(c);
    int ans;
    switch(c -> n){
        case I2C1:
            while(I2C1STATbits.TBF){}
            I2C1TRN = data;
            if(I2C1STATbits.IWCOL)        
                ans = -1;
            else{
                idleI2C(c); 
                if(I2C1STATbits.ACKSTAT) ans = -2;
                else ans =  0;
            }
            break;
        case I2C2:
            while(I2C2STATbits.TBF){}
            I2C2TRN = data;
            if(I2C2STATbits.IWCOL) ans = -1;
            else{
                idleI2C(c);                  
                if( I2C2STATbits.ACKSTAT ) 
                    ans = -2;
                else ans =  0;
            }
            break;
        case I2C4:
            while(I2C4STATbits.TBF){}
            I2C4TRN = data;
            if(I2C4STATbits.IWCOL)        
                ans = -1;
            else{
                idleI2C(c);                  
                if( I2C4STATbits.ACKSTAT ) 
                    ans = -2;
                else ans =  0;
            }
            break;  

    }
    return ans;
}   

int i2cWriteString(i2c* c, char * s){
    while(*s){
        if(i2cWrite(c, *s) == -1)
            return -3; 
        switch (c->n){
            case I2C1:   while(I2C1STATbits.TBF); break;
            case I2C2:   while(I2C2STATbits.TBF); break;
            case I2C4:   while(I2C4STATbits.TBF); break;
        }
        idleI2C(c);
        s++;
    }
    return 0;
}


unsigned char i2cRead(i2c* c){
    idleI2C(c);
    unsigned char ans;
    switch (c->n){
        case I2C1:    I2C1CONbits.RCEN = 1;  while(I2C1CONbits.RCEN){}; 
                        I2C1STATbits.I2COV = 0; while(!I2C1STATbits.RBF){}; 
                        ans = I2C1RCV; break;
        case I2C2:    I2C2CONbits.RCEN = 1; while(I2C2CONbits.RCEN){};
                        I2C2STATbits.I2COV = 0; ans = I2C2RCV; break;
        case I2C4:    I2C4CONbits.RCEN = 1; while(I2C4CONbits.RCEN){};
                        I2C4STATbits.I2COV = 0; ans = I2C4RCV; break;

    }
    return ans;
}

int i2cReadString(i2c* c, char* s, int len){
    while(len){
        *s = i2cRead(c);
        
        s++;  len--;
        if(len == 0)
            i2cSendNACK(c);
        else
            i2cSendACK(c);

        idleI2C(c);
    }
    return 0;
}


void i2cSendACK(i2c* c){
    idleI2C(c);
    switch(c -> n){
        case I2C1: I2C1CONbits.ACKDT = 0, I2C1CONbits.ACKEN = 1; break;
        case I2C2: I2C2CONbits.ACKDT = 0, I2C2CONbits.ACKEN = 1; break;
        case I2C4: I2C4CONbits.ACKDT = 0, I2C4CONbits.ACKEN = 1; break;
    }
}

void i2cSendNACK(i2c* c){
    idleI2C(c);
    switch(c -> n){
        case I2C1: I2C1CONbits.ACKDT = 1, I2C1CONbits.ACKEN = 1; break;
        case I2C2: I2C2CONbits.ACKDT = 1, I2C2CONbits.ACKEN = 1; break;
        case I2C4: I2C4CONbits.ACKDT = 1, I2C4CONbits.ACKEN = 1; break;
    }
}

char i2cAvailable(i2c* c){
    char ans;
    switch(c -> n){
        case I2C1: ans = I2C1STATbits.RBF; break;
        case I2C2: ans = I2C2STATbits.RBF; break;
        case I2C4: ans = I2C4STATbits.RBF; break;
    }
    return ans;
}

int i2cStartWrite(i2c* c){
    return i2cWrite(c, (c->address << 1));
}

int i2cStartRead(i2c* c){
    return i2cWrite(c, (c->address << 1) + 1);
}