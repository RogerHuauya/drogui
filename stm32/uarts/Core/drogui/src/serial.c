#include "serial.h"

#include <stdarg.h>
#include <stdio.h>

char serial_buffer[100];

serial serial2, serial3, serial4, serial5; 


void serialPrintf(serial *ser, const char *s, ...) {
    va_list args;
    va_start(args, s);
    vsprintf(serial_buffer, s, args);
    va_end(args);
    serialPrint(ser, serial_buffer);
}

void serialBegin(serial* ser, volatile uint32_t * cr, volatile uint32_t * brr){
    ser->cr = cr;
    ser->brr = brr;
    ser->rcv_head = ser->snd_head = 0;
    ser->rcv_tail = ser->snd_tail = 0;
}

void serialsBegin(){
    serialBegin(&serial2, &(USART2->CR1), &(USART2->BRR));
    serialBegin(&serial3, &(USART3->CR1), &(USART3->BRR));
    serialBegin(&serial4, &(UART4->CR1), &(UART4->BRR));
    serialBegin(&serial5, &(UART5->CR1), &(UART5->BRR));
}



void serialPrint(serial* ser, char *s){
    while(*s){
        ser -> snd_buff[(ser->snd_head)++] = *s, s++;
        ser -> snd_head %= SER_BUFF_SZ;
        ser -> snd_tail += (ser->snd_head == ser->snd_tail);
        ser -> snd_tail %= SER_BUFF_SZ;
    }
    *(ser->cr)  |= USART_CR1_TXEIE;
}

void serialWrite(serial*ser, char c){
    
	*(ser->cr)  &= ~(USART_CR1_TXEIE);
    ser->snd_buff[(ser->snd_head)++] = c;
    ser->snd_head %= SER_BUFF_SZ;
    ser->snd_tail += (ser->snd_head == ser->snd_tail);
    ser->snd_tail %= SER_BUFF_SZ;
    *(ser->cr)  |= USART_CR1_TXEIE;
}

bool serialAvailable(serial* ser){
    return ser->rcv_head != ser->rcv_tail;
}

char serialRead(serial *ser){
    char ans;	
    *(ser->cr)  &= ~(USART_CR1_RXNEIE);	
    ans = ser->rcv_buff[(ser->rcv_tail)++];
    ser->rcv_tail %= SER_BUFF_SZ;
    *(ser->cr)  |= USART_CR1_RXNEIE;
    return ans;
}

void serialFlush(serial *ser){
    *(ser->cr)  &= ~(USART_CR1_RXNEIE);	
    ser->rcv_tail = ser->rcv_head;
    *(ser->cr)  |= USART_CR1_RXNEIE;
}


void changeBaudrate(serial* ser, int baudrate){
    *(ser->cr) ^= USART_CR1_UE;

    *(ser->brr) = (uint16_t)(UART_DIV_SAMPLING16( HAL_RCC_GetPCLK1Freq(), baudrate));
    
    *(ser->cr) |= USART_CR1_UE;
}