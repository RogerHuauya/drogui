#ifndef SERIAL_H
#define SERIAL_H
#include "config.h"
#include <xc.h>
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define BUFF_LENGTH 100

#define uartRxInterrupt(n, pr) __attribute__((vector(_UART##n##_RX_VECTOR), interrupt(ipl##pr##auto), nomips16)) _U##n##RXInterrupt(void)
#define uartTxInterrupt(n, pr) __attribute__((vector(_UART##n##_TX_VECTOR), interrupt(ipl##pr##auto), nomips16)) _U##n##TXInterrupt(void)

void initSerial(int baudrate, int priority);
bool serialAvailable();
void serialWriteChar(char data);
void serialWriteString(char* s);
char serialReadChar();
long long serialParseInt();
void serialReadString(char *s, int timeout);


#endif