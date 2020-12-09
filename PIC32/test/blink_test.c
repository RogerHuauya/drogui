//#define BLINK_TEST
#ifdef BLINK_TEST

#include "config.h"
#include <xc.h>
#include "io.h"

#define LED1 PRTC, 4
#define LED2 PRTC, 1

int main(void){
    
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    
    while (true){

        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        delayMs(10);
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        delayMs(10);
    }
    return 0;
}
#endif