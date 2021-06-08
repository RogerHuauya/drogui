//#define IO_TEST
#ifdef IO_TEST

#include "config.h"
#include "utils.h"
#include "io.h"

#define DIR PRTC,1
#define STP PRTC,4

int main(){
    
    pinMode(LED1, OUTPUT);
    pinMode(STP, OUTPUT);
    pinMode(DIR, OUTPUT);
    digitalWrite(STP, HIGH);
    digitalWrite(DIR, LOW);
    
    while(1){
        digitalToggle(LED1);
        digitalToggle(STP);
        digitalToggle(DIR);
        delayMs(100);
    }
}



#endif