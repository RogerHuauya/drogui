#ifndef SERIAL_H
#define SERIAL_H
#include "config.h"
#include <xc.h>
#include <stdbool.h>
#include "utils.h"
#define BUFF_LENGTH 32

#define uartRxInterrupt(n) __attribute__((interrupt, no_auto_psv)) _U##n##RXInterrupt(void)

void initSerial();
bool serialAvailable();
void serialWriteChar(char data);
void serialWriteString(char* s);
char serialReadChar();

long long serialParseInt();



#endif