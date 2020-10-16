#include "serial.h"

serial *s1, *s2;


void serialWriteChar(serial *s, char data){
    switch(s -> n){
     case SERIAL1: while(U1STAbits.UTXBF){} U1TXREG = data; break;
     case SERIAL2: while(U2STAbits.UTXBF){} U2TXREG = data; break;
    }
}

void serialWriteString(serial *s, char* str){
    while(*str){
        serialWriteChar(s, *str);
        str++;
    }
}

void initSerial(serial* s, int n, long long baudrate){
    s -> n = n;

    switch(n){
    
        case SERIAL1:
            s1 = s;
            RPINR18bits.U1RXR = 97;
            RPOR3bits.RP71R = 1;

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
            
            U1BRG = ((FCY/baudrate)/4)-1;
            
            break;
        
        case SERIAL2:
            s2 = s;
            RPINR19bits.U2RXR = 74;
            //RPOR3bits.RP71R = 1;

            U2MODEbits.UARTEN = 1;
            U2MODEbits.UEN = 0;
            U2MODEbits.URXINV = 0;
            U2MODEbits.PDSEL = 0;
            U2MODEbits.STSEL = 0;
            U2MODEbits.BRGH = 1;
            
            U2STAbits.UTXEN = 1;
            U2STAbits.URXISEL = 0;

            IPC7bits.U2RXIP = 4;
            IFS1bits.U2RXIF = 0; 
            IEC1bits.U2RXIE = 1;
            
            U2BRG = ((FCY/baudrate)/4)-1;
            
            break;

    }
    __delay_ms(1000);

}


bool serialAvailable(serial * s){
    return (s->buff_head) != (s->buff_tail);
}

char serialReadChar(serial *s){
    (s -> buff_tail) = ((s -> buff_tail) + 1)%BUFF_LENGTH;
    return s -> buf[ ((s -> buff_tail) + BUFF_LENGTH - 1) % BUFF_LENGTH];
}


void uartRxInterrupt(1){
    while(U1STAbits.URXDA){
        s1->buf[ (s1->buff_head)++] = U1RXREG;
        (s1->buff_head) %= BUFF_LENGTH;
        (s1->buff_tail) += ((s1->buff_head) == (s1->buff_tail));
        (s1->buff_tail) %= BUFF_LENGTH;
    }
    IFS0bits.U1RXIF = 0; 
}

void uartRxInterrupt(2){
    while(U2STAbits.URXDA){
        s2->buf[ (s2->buff_head)++] = U2RXREG;
        (s2->buff_head) %= BUFF_LENGTH;
        (s2->buff_tail) += ((s2->buff_head) == (s2->buff_tail));
        (s2->buff_tail) %= BUFF_LENGTH;
    }
    IFS1bits.U2RXIF = 0; 
}

int serialParseInt(serial* s, long long *ans){
    *ans = 0LL;
    char c = '*';
    bool flag = false;
    long long t;
    while(1){
        while(!serialAvailable(s) && (++t) < 10000LL);
        if(t >= 10000LL) return -1;
        c = serialReadChar(s);
        if(c == '-') flag = true;
        else if(c == '$') break;
        else{
            *ans *= 10LL;
            *ans += (int) (c - '0');
        }
    }
    if(flag) *ans *= -1LL;
    return 0;
}

void serialFlush(serial* s){
    s -> buff_head = s -> buff_tail;
}

