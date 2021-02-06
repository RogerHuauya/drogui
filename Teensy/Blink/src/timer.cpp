#include "..\headers\timer.h"

void initTimer(timer* t, void (*f) (), int freq){
    t-> time = 0;
    t->period = 1000000/freq;
    t->function = f;
}

bool timerReady(timer* t){
    return t->time >= t->period;
}

void executeTimer(timer* t){
    t -> time = 0;
    t -> function();
}
