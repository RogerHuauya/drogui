#ifndef SERIAL_H
#define SERIAL_H
#include <xc.h>
#include <stdbool.h>


void initSerial();
bool serialAvailable();
void serialWriteChar(char data);
void serialWriteString(char* s, int nbytes);
char serialReadChar();
#endif