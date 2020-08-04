#include "io.h"

void digitalWrite(int port, int pin, bool on){
    if(on){
        switch (port){
            case PRTA:  LATA |= (1 << pin); break;
            case PRTB:  LATB |= (1 << pin); break;
            case PRTC:  LATC |= (1 << pin); break;
            case PRTD:  LATD |= (1 << pin); break;
            case PRTE:  LATE |= (1 << pin); break;
            case PRTF:  LATF |= (1 << pin); break;
            case PRTG:  LATG |= (1 << pin); break;
        } 
    }
    else{
        switch (port){
            case PRTA:  LATA &= ~(1 << pin); break;
            case PRTB:  LATB &= ~(1 << pin); break;
            case PRTC:  LATC &= ~(1 << pin); break;
            case PRTD:  LATD &= ~(1 << pin); break;
            case PRTE:  LATE &= ~(1 << pin); break;
            case PRTF:  LATF &= ~(1 << pin); break;
            case PRTG:  LATG &= ~(1 << pin); break;
        }
    }
}

void digitalToggle(int port, int pin){
    switch (port){
        case PRTA:  LATA ^= (1 << pin); break;
        case PRTB:  LATB ^= (1 << pin); break;
        case PRTC:  LATC ^= (1 << pin); break;
        case PRTD:  LATD ^= (1 << pin); break;
        case PRTE:  LATE ^= (1 << pin); break;
        case PRTF:  LATF ^= (1 << pin); break;
        case PRTG:  LATG ^= (1 << pin); break;
    }
}


void portDigitalWrite(int port, int values){
    switch (port){
        case PRTA: LATA |= values; break;
        case PRTB: LATB |= values; break;
        case PRTC: LATC |= values; break;
        case PRTD: LATD |= values; break;
        case PRTE: LATE |= values; break;
        case PRTF: LATF |= values; break;
        case PRTG: LATG |= values; break;
    } 
}


bool digitalRead(int port, int pin){
    bool ans;
    switch (port){
        case PRTA: ans = PORTA & (1<<pin); break;
        case PRTB: ans = PORTB & (1<<pin); break;
        case PRTC: ans = PORTC & (1<<pin); break;
        case PRTD: ans = PORTD & (1<<pin); break;
        case PRTE: ans = PORTE & (1<<pin); break;
        case PRTF: ans = PORTF & (1<<pin); break;
        case PRTG: ans = PORTG & (1<<pin); break;
    } 
    return ans;
}

int portDigitalRead(int port){
    int ans;
    switch (port){
        case PRTA: ans = PORTA; break;
        case PRTB: ans = PORTB; break;
        case PRTC: ans = PORTC; break;
        case PRTD: ans = PORTD; break;
        case PRTE: ans = PORTE; break;
        case PRTF: ans = PORTF; break;
        case PRTG: ans = PORTG; break;
    } 
    return ans;
}


void pinMode(int port, int pin, bool in){
    bool change = true;
    switch (port){
        case PRTA:  change = (TRISA >> pin) ^ in; break;
        case PRTB:  change = (TRISB >> pin) ^ in; break;
        case PRTC:  change = (TRISC >> pin) ^ in; break;
        case PRTD:  change = (TRISD >> pin) ^ in; break;
        case PRTE:  change = (TRISE >> pin) ^ in; break;
        case PRTF:  change = (TRISF >> pin) ^ in; break;
        case PRTG:  change = (TRISG >> pin) ^ in; break;
    } 
    
    if(!change) return;
    
    switch (port){
        case PRTA:  TRISA ^= (1 << pin); break;
        case PRTB:  TRISB ^= (1 << pin); break;
        case PRTC:  TRISC ^= (1 << pin); break;
        case PRTD:  TRISD ^= (1 << pin); break;
        case PRTE:  TRISE ^= (1 << pin); break;
        case PRTF:  TRISF ^= (1 << pin); break;
        case PRTG:  TRISG ^= (1 << pin); break;
    } 
}