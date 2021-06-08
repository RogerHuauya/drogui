#include "io.h"

void digitalWrite(int port, int pin, bool on){
    int num = ( ( (int) on ) << pin);
    switch (port){
        case PRTA: LATA |= num; break;
        case PRTB: LATB |= num; break;
        case PRTC: LATC |= num; break;
    }
}

void portDigitalWrite(int port, int values){
    switch (port){
        case PRTA: LATA |= values; break;
        case PRTB: LATB |= values; break;
        case PRTC: LATC |= values; break;
    } 
}


bool digitalRead(int port, int pin){
    bool ans;
    switch (port){
        case PRTA: ans = PORTA & (1<<pin); break;
        case PRTB: ans = PORTB & (1<<pin); break;
        case PRTC: ans = PORTC & (1<<pin); break;
    } 
    return ans;
}

int portDigitalRead(int port){
    int ans;
    switch (port){
        case PRTA: ans = PORTA; break;
        case PRTB: ans = PORTB; break;
        case PRTC: ans = PORTC; break;
    } 
    return ans;
}


