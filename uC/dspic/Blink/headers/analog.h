#ifndef ANALOG_H
#define ANALOG_H
#include "config.h"
#include <xc.h>
#include <libpic30.h>

void initAnalog();
uint16_t analogRead(int);

#endif