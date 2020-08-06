#include "serial.h"

char str[20];

char buf[BUFF_LENGTH];
int buff_head = 0, buff_tail = 0;

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
    RPINR18bits.U1RXR = 112;
    RPOR13bits.RP113R = 1;

    U1MODEbits.UARTEN = 1;
    U1MODEbits.UEN = 0;
    U1MODEbits.URXINV = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    U1MODEbits.BRGH = 1;
     
    U1STAbits.UTXEN = 1;
    U1STAbits.URXISEL = 0;

    IPC2bits.U1RXIP = 4;
    IFS0bits.U1RXIF = 0; 
    IEC0bits.U1RXIE = 1;
    
    U1BRG = BRGVAL;
}


bool serialAvailable(){
    return buff_head != buff_tail;
}

char serialReadChar(){
    buff_tail = (buff_tail + 1)%BUFF_LENGTH;
    return buf[ (buff_tail + BUFF_LENGTH - 1) % BUFF_LENGTH];
}

void uartRxInterrupt(1){

    while(U1STAbits.URXDA){
        buf[buff_head++] = U1RXREG;
        buff_head %= BUFF_LENGTH;
        buff_tail += (buff_head == buff_tail);
        buff_tail %= BUFF_LENGTH;
    }
    IFS0bits.U1RXIF = 0; 
}

long long serialParseInt(){
    long long ans = 0LL;
    char c = '*';
    bool flag = false;

    while(1){
        while(!serialAvailable());
        c = serialReadChar();
        if(c == '-') flag = true;
        else if(c == '$') break;
        else{
            ans *= 10LL;
            ans += (int) (c - '0');
        }
    }
    if(flag) ans *= -1LL;
    return ans;
}