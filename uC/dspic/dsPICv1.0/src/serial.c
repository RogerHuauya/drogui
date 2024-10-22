#include "serial.h"
#include "config.h"

void serialWriteChar(char data){
    while(U1STAbits.UTXBF){}
    U1TXREG = data;
}

void serialWriteString(char* s){
    while(*s){
        serialWriteChar(*s);
        s++;
    }
}

void initSerial(){

    RPINR18bits.U1RXR = 20;
    RPOR10bits.RP21R = 3;

    U1MODEbits.UARTEN = 1;
    U1MODEbits.UEN = 0;
    U1MODEbits.URXINV = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 1;
    U1MODEbits.BRGH = 1;

    U1BRG = BRGVAL;
}

bool serialAvailable(){
    return U1STAbits.URXDA;
}

char serialReadChar(){
    return U1RXREG;
}
