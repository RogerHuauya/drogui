#ifndef SERIAL_H
#define SERIAL_H

#include "utils.h"

#define SER_BUFF_SZ 300

typedef struct _serial{
    uint8_t rcv_buff[SER_BUFF_SZ], rcv_head, rcv_tail;
    uint8_t snd_buff[SER_BUFF_SZ], snd_head, snd_tail;
    volatile uint32_t* cr, *brr;
} serial;



extern serial serial2, serial3, serial4, serial5;

void serialsBegin();
void serialBegin(serial* ser, volatile uint32_t * cr, volatile uint32_t * brr);
void serialPrint(serial* ser, char *s);
void serialPrintf(serial* ser, const char *s, ...);
void serialWrite(serial* ser, char c);

bool serialAvailable(serial* ser);
char serialRead(serial* ser);
void changeBaudrate(serial* ser, int baudrate);
void serialFlush(serial* ser);

#endif