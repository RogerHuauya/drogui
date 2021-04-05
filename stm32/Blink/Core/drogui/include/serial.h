#ifndef SERIAL_H
#define SERIAL_H

#include "utils.h"


void serialPrint(char *s);
void serialPrintf(const char *s, ...);

bool serialAvailable();
char serialReadChar();

#endif