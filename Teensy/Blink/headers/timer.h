#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>
struct timer{
    elapsedMicros time;
    unsigned int period;
    void (*function) ();
};

void initTimer(timer* t, void (*f) (), int freq);
bool timerReady(timer* t);
void executeTimer(timer* t);

#endif