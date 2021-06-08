#ifndef IO_H
#define IO_H
#include <xc.h>
#include <stdbool.h>

enum ports {PRTA, PRTB, PRTC, PRTD, PRTE, PRTF, PRTG};
enum pin_state {LOW, HIGH};
enum pin_mode  {OUTPUT, INPUT};


void digitalWrite(int port, int pin, bool on);
void portDigitalWrite(int port, int ports);

bool digitalRead(int port, int pin);
int portDigitalRead(int port);

void pinMode(int port, int pin, bool in);
void digitalToggle(int port, int pin);


#endif