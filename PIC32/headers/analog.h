#ifndef ANALOG_H
#define ANALOG_H
#include "config.h"
#include <xc.h>
#include "utils.h"

void initAnalog();
uint16_t analogRead(int);

#endif