//#define SENSORS
#ifdef SENSORS

#include <xc.h>
#include "sensors.h"
#include "config.h"
#include "serial.h"
#include "io.h"
#define LED PRTB, 11

press ppiston;
flow lily;

char s[70];

int main(){
    initPBCLK();
    initSerial(115200, 4);
    initPress(&ppiston, I2C1);
    initFlow(&lily, I2C1);
    pinMode(LED, OUTPUT);       
    digitalWrite(LED, HIGH);
    while(1){
        
        readPress(&ppiston);
        readFlow(&lily);

        sprintf(s, "%6.2lf  %6.2lf\n", ppiston.val, lily.val);
        serialWriteString(s);
        digitalToggle(LED);
        delayMs(100);
    }

    return 0;
}


#endif