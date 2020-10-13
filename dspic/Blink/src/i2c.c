#include "i2c.h"
#include "serial.h"

bool on1 = false, on2 = false , on3 = false;
uint8_t temp, datain, dataout;
int i2c1State = 0, i2c2State = 0;
int err_aux = 0;
uint8_t i2c1Reg[20];
uint8_t i2c2Reg[20];

int idleI2C(i2c *c){
    int ans = SUCCESS, timeout = 0;
    switch(c -> n){
        case I2C1: while((I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || I2C1CONbits.RSEN
                            || I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT) && timeout < TIME_OUT){timeout++;} break;
        case I2C2: while((I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RCEN || I2C2CONbits.RSEN
                            || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT) && timeout < TIME_OUT){timeout++;} break;
    }
    if(timeout == TIME_OUT) ans = TIMEOUT_ERROR;
    return ans;
}

void rebootI2C(i2c *c){
    switch(c-> n){
    case I2C1:    I2C1CONbits.SEN = I2C1CONbits.PEN = I2C1CONbits.RCEN \
                    = I2C1CONbits.RSEN = I2C1CONbits.ACKEN = I2C1STATbits.TRSTAT = 0;
                    I2C1CONbits.I2CEN = 0; __delay_us(10); I2C1CONbits.I2CEN = 1;
                    break;
    case I2C2:    I2C2CONbits.SEN = I2C2CONbits.PEN = I2C2CONbits.RCEN \
                    = I2C2CONbits.RSEN = I2C2CONbits.ACKEN = I2C2STATbits.TRSTAT = 0;
                    break;
    }
    i2cStop(c);
}


void initI2C(i2c* c, int n, uint8_t address, double freq, int mode){
    c -> n = n;
    c -> address = address;
    c -> mode = mode;
    int BRG = ((1 / freq) - 130e-9) * FCY - 2;
    if (c -> mode == SLAVE){
        switch (c->n){
            case I2C1: I2C1ADD = c->address; IEC1bits.MI2C1IE = 1; IEC1bits.SI2C1IE = 1; I2C1CONbits.STREN = 1;
            case I2C2: I2C2ADD = c->address; IEC3bits.MI2C2IE = 1; IEC3bits.SI2C2IE = 1; I2C2CONbits.STREN = 1;
        }

    }
    switch(c->n){
        case I2C1: if(on1) break; I2C1BRG = BRG; I2C1CONbits.DISSLW = 1; I2C1CONbits.I2CEN = 1; on1 = 1; break;
        case I2C2: if(on2) break; I2C2BRG = BRG; I2C2CONbits.DISSLW = 0; I2C2CONbits.I2CEN = 1; on2 = 1; break;
    } 
    return;
}


int i2cStart(i2c* c){
    err_aux = idleI2C(c);
    if(err_aux != SUCCESS) return err_aux;
    
    switch(c -> n){
        case I2C1: I2C1CONbits.SEN = 1; break;
        case I2C2: I2C2CONbits.SEN = 1; break;
    }
}

int i2cStop(i2c* c){
    err_aux = idleI2C(c);
    if(err_aux != SUCCESS) return err_aux;
    
    switch(c -> n){
        case I2C1: I2C1CONbits.PEN = 1; break;
        case I2C2: I2C2CONbits.PEN = 1; break;
    }
}

int i2cRestart(i2c* c){
    err_aux = idleI2C(c);
    if(err_aux != SUCCESS) return err_aux;
    
    switch(c -> n){
        case I2C1: I2C1CONbits.RSEN = 1; break;
        case I2C2: I2C2CONbits.RSEN = 1; break;
    }
}

int i2cWrite(i2c* c, uint8_t data){
    idleI2C(c);
    int ans, timeout = 0;
    switch(c -> n){
        case I2C1:
            while(I2C1STATbits.TBF && timeout < TIME_OUT){timeout++;}
            if(I2C1STATbits.TBF){ ans = TMR_FULL_ERROR; break;}
            timeout = 0;
            I2C1TRN = data;
            if(I2C1STATbits.IWCOL) ans = COL_ERROR;
            else{      
                err_aux = idleI2C(c);
                if(err_aux != SUCCESS){ ans = err_aux; break;}
                while(I2C1STATbits.ACKSTAT && timeout < TIME_OUT){timeout++;}
                if(I2C1STATbits.ACKSTAT) ans = ACK_ERROR;
                else ans = SUCCESS;
            }
            break;
        case I2C2:
            
            while(I2C2STATbits.TBF && timeout < TIME_OUT){timeout++;}
            if(I2C2STATbits.TBF){ans = TMR_FULL_ERROR; break;}
            timeout = 0;
            I2C2TRN = data;
            
            if(I2C2STATbits.IWCOL) ans = COL_ERROR;
            else{
                err_aux = idleI2C(c);        
                if(err_aux != SUCCESS){ ans = err_aux; break;}          
                while(I2C2STATbits.ACKSTAT && timeout < TIME_OUT) {timeout++;}
                if(I2C2STATbits.ACKSTAT) ans = ACK_ERROR;
                else ans = SUCCESS;
            }

            break;
    }
    return ans;
}   

int i2cWriteString(i2c* c, uint8_t * s){
    int ans = SUCCESS;
    int timeout = 0;
    while(*s){
        err_aux = i2cWrite(c, *s); 
        if(err_aux != SUCCESS){ ans = err_aux; break;} 
        
        switch (c->n){
            case I2C1: while(I2C2STATbits.TBF && timeout < TIME_OUT){timeout++;}; 
                       if(I2C2STATbits.TBF) ans = TMR_FULL_ERROR; break;
            case I2C2: while(I2C2STATbits.TBF && timeout < TIME_OUT){timeout++;}; 
                       if(I2C2STATbits.TBF) ans = TMR_FULL_ERROR; break;
        }

        if(err_aux != SUCCESS) break;
        
        err_aux = idleI2C(c);
        if(err_aux != SUCCESS){ans = err_aux; break;}
        
        s++;
    }
    return ans;
}


int i2cRead(i2c* c, uint8_t* data){
    idleI2C(c);
    int timeout = 0, ans;
    switch (c->n){
        case I2C1:    I2C1CONbits.RCEN = 1; 
                        while(I2C1CONbits.RCEN && timeout < TIME_OUT){timeout++;}
                        if(I2C1CONbits.RCEN){ ans = RCV_ERROR; break;}
                        I2C1STATbits.I2COV = 0; timeout = 0;
                        while(!I2C1STATbits.RBF && timeout < TIME_OUT){timeout++;}
                        if(!I2C1STATbits.RBF){ans = RBF_ERROR; break;} 
                        *data = I2C1RCV; break;
                        
        case I2C2:    I2C2CONbits.RCEN = 1; 
                        while(I2C2CONbits.RCEN && timeout < TIME_OUT){timeout++;}
                        if(I2C2CONbits.RCEN){ ans = RCV_ERROR; break;}
                        I2C2STATbits.I2COV = 0; timeout = 0;
                        while(!I2C2STATbits.RBF && timeout < TIME_OUT){timeout++;}
                        if(!I2C2STATbits.RBF){ans = RBF_ERROR; break;} 
                        *data = I2C2RCV; break;
    }
    return SUCCESS;
}

int i2cReadString(i2c* c, uint8_t* s, int len){
    int ans = SUCCESS;
    while(len){
        err_aux = i2cRead(c, s);
        if(err_aux != SUCCESS){ans = err_aux; break;}
        
        s++;  len--;
        if(len == 0)
            err_aux = i2cSendNACK(c);
            if(err_aux != SUCCESS) return err_aux;
        else
            err_aux = i2cSendACK(c);
            if(err_aux != SUCCESS) return err_aux;

        err_aux = idleI2C(c);
        if(err_aux != SUCCESS){ans = err_aux; break;}
    }
    return ans;
}


int i2cSendACK(i2c* c){
    err_aux = idleI2C(c);
    if(err_aux != SUCCESS) return err_aux;
    
    switch(c -> n){
        case I2C1: I2C1CONbits.ACKDT = 0, I2C1CONbits.ACKEN = 1; break;
        case I2C2: I2C2CONbits.ACKDT = 0, I2C2CONbits.ACKEN = 1; break;
    }
}

int i2cSendNACK(i2c* c){
    err_aux = idleI2C(c);
    if(err_aux != SUCCESS) return err_aux;

    switch(c -> n){
        case I2C1: I2C1CONbits.ACKDT = 1, I2C1CONbits.ACKEN = 1; break;
        case I2C2: I2C2CONbits.ACKDT = 1, I2C2CONbits.ACKEN = 1; break;
    }

}

int i2cStartWrite(i2c* c){
    return i2cWrite(c, (c->address << 1));
}

int i2cStartRead(i2c* c){
    return i2cWrite(c, (c->address << 1) + 1);
}



void __attribute__ ( (interrupt, no_auto_psv) ) _SI2C1Interrupt( void ){

    if (I2C1STATbits.P == 1) i2c1State = 0;
    
    if( (I2C1STATbits.R_W == 0) && (I2C1STATbits.D_A == 0) ){
        temp = I2C1RCV; 
        //__delay_ms(1000);
        //serialWriteString("addrress match write\n");
        I2C1CONbits.SCLREL = 1;
        
    }
    else if( (I2C1STATbits.R_W == 0) && (I2C1STATbits.D_A == 1) ) {
        
        if(i2c1State == 0){
            datain = I2C1RCV;
            //serialWriteString("datain received\n");
            i2c1State++;

        } 
        else {
            i2c1Reg[datain++] = I2C1RCV; 
            i2c1State = 0;
            //serialWriteString("register changed\n");
        }
        //__delay_ms(1000);

        I2C1CONbits.SCLREL = 1;
    }
    else if( (I2C1STATbits.R_W == 1) && (I2C1STATbits.D_A == 0) ) {
        temp = I2C1RCV;
        dataout = i2c1Reg[datain++];
        I2C1TRN = dataout;
        //serialWriteString("addres match read\n");
        //__delay_ms(1000);
        I2C1CONbits.SCLREL = 1;
    
    }
    else if ( (I2C1STATbits.R_W == 1) && (I2C1STATbits.D_A == 1) && (I2C1STATbits.ACKSTAT == 0 )){
        //serialWriteString("register sent\n");
        temp = I2C1RCV;
        dataout = i2c1Reg[datain++];
        I2C1TRN = dataout;
        //__delay_ms(1000);
        I2C1CONbits.SCLREL = 1; 
    }
    
    _SI2C1IF = 0;
 
}

void __attribute__ ( (interrupt, no_auto_psv) ) _SI2C2Interrupt( void ){

    if (I2C2STATbits.P == 1) i2c2State = 0;
    
    if( (I2C2STATbits.R_W == 0) && (I2C2STATbits.D_A == 0) ){
        temp = I2C2RCV; 
        //__delay_ms(1000);
        //serialWriteString("addrress match write\n");
        I2C2CONbits.SCLREL = 1;
        
    }
    else if( (I2C2STATbits.R_W == 0) && (I2C2STATbits.D_A == 1) ) {
        
        if(i2c2State == 0){
            
            datain = I2C2RCV;
            i2c2State++;

        } 
        else {
            i2c2Reg[datain++] = I2C2RCV; 
            i2c2State = 0;
            //serialWriteString("register value changed\n");
        }
        //__delay_ms(1000);

        I2C2CONbits.SCLREL = 1;
    }
    else if( (I2C2STATbits.R_W == 1) && (I2C2STATbits.D_A == 0) ) {
        temp = I2C2RCV;
        dataout = i2c2Reg[datain++];
        I2C2TRN = dataout;
        i2c2State = 0;
        //serialWriteString("addres match read\n");
        //__delay_ms(1000);
        I2C2CONbits.SCLREL = 1;
    
    }
    else if ( (I2C2STATbits.R_W == 1) && (I2C2STATbits.D_A == 1) && (I2C2STATbits.ACKSTAT == 0 )){
        //serialWriteString("register read \n");
        temp = I2C2RCV;
        dataout = i2c2Reg[datain++];
        I2C2TRN = dataout;
        //__delay_ms(1000);
        I2C2CONbits.SCLREL = 1; 
    }
    
    _SI2C2IF = 0;
 
}