#ifndef IO_H
#define IO_H
#include <xc.h>
#include <stdbool.h>

enum ports {PRTA, PRTB, PRTC, PRTD};


void digitalWrite(int port, int pin, bool on);
void portDigitalWrite(int port, int ports);

bool digitalRead(int port, int pin);
int portDigitalRead(int port);


#endif