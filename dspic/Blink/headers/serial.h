#ifndef SERIAL_H
#define SERIAL_H
#include "config.h"
#include <xc.h>
#include <stdbool.h>
#include "utils.h"
#define BUFF_LENGTH 32

#define uartRxInterrupt(n) __attribute__((interrupt, no_auto_psv)) _U##n##RXInterrupt(void)
enum SERIALS {SERIAL1, SERIAL2};

typedef struct _serial{    
    char buf[BUFF_LENGTH];
    int buff_head, buff_tail, n;
} serial;



void initSerial(serial *s, int n, long long baudrate);
bool serialAvailable(serial *s);
void serialWriteChar(serial *s, char data);
void serialWriteString(serial *s, char* str);
char serialReadChar(serial *s);

long long serialParseInt(serial *s);

void serialFlush(serial* s);

#endif