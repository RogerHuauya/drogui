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

void serialWrite(char c){
    
	USART2->CR1  &= ~(USART_CR1_TXEIE);
    snd_buff[snd_head++] = c;
    snd_head %= SER_BUFF_SZ;
    snd_tail += (snd_head == snd_tail);
    snd_tail %= SER_BUFF_SZ;
    USART2->CR1  |= USART_CR1_TXEIE;
}

bool serialAvailable(){
    return rcv_head != rcv_tail;
}

char serialRead(){
    char ans;	
    USART2->CR1  &= ~(USART_CR1_RXNEIE);	
    ans = rcv_buff[rcv_tail++];
    rcv_tail %= SER_BUFF_SZ;
    USART2->CR1  |= USART_CR1_RXNEIE;
    return ans;
}

void serialFlush(){
    USART2->CR1  &= ~(USART_CR1_RXNEIE);	
    rcv_tail = rcv_head;
    USART2->CR1  |= USART_CR1_RXNEIE;
}


void changeBaudrate(int baudrate){
    USART2 -> CR1 ^= USART_CR1_UE;

    USART2 -> BRR = (uint16_t)(UART_DIV_SAMPLING16( HAL_RCC_GetPCLK1Freq(), baudrate));
    
    USART2 -> CR1 |= USART_CR1_UE;
}