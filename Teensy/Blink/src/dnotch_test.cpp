//#define DNOTCH_TEST
#ifdef DNOTCH_TEST

#include "..\headers\timer.h"
#include "..\headers\filter.h"
#include "..\headers\main.h"
#include <Arduino.h>

dNotchFilter dn;
int n = 64;
float data[] = {1,1,1,1,0,0,0,0};

timer timer_filt;
int c = 0;
void fftInterrupt(){

    Serial.print(computeDNotch(&dn, data[c]), DEC);
    Serial.println(';');     
    Serial.println("];");
}

void initializeSystem(){

    initDNotchFilter(&dn, n, 50, 1000, 1, 5);
    initTimer(&timer_filt, &fftInterrupt, 10000);
    Serial.println("arduinoData =[");

    delay(500);
}


int _main(void){
    
    initializeSystem();
    delay(1000);
    while(1){
        if(c  == sizeof(data)/sizeof(data[0])) while(1);
        if(timerReady(&timer_filt)) Serial.print(timer_filt.time), Serial.print(" "), executeTimer(&timer_filt), c++;
    }
    
    return 0;
}










#endif