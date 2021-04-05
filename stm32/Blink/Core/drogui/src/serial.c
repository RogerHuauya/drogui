#include "serial.h"

#include <stdarg.h>
#include <stdio.h>
char serial_buffer[100];

void serialPrintf(const char *s, ...) {
    va_list args;
    va_start(args, s);
    vsprintf(serial_buffer, s, args);
    va_end(args);
    serialPrint(serial_buffer);
}


void serialPrint(char *s){
    while(*s){
        snd_buff[snd_head++] = *s, s++;
        snd_head %= SER_BUFF_SZ;
        snd_tail += (snd_head == snd_tail);
        snd_tail %= SER_BUFF_SZ;
    }

    USART2->CR1  |= USART_CR1_TXEIE;
}

bool serialAvailable(){
    return rcv_head != rcv_tail;
}

char serialReadChar(){
    rcv_tail = (rcv_tail + 1)%SER_BUFF_SZ;
    return rcv_buff[ (rcv_tail + SER_BUFF_SZ - 1) % SER_BUFF_SZ];
}