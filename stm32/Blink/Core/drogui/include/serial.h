#ifndef SERIAL_H
#define SERIAL_H

#include "utils.h"


void serialPrint(char *s);
void serialPrintf(const char *s, ...);
void serialWrite(char c);

bool serialAvailable();
char serialRead();
void changeBaudrate(int baudrate);

#endif