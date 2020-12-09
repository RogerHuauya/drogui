#include "utils.h"
#include "config.h"


void delayUs(unsigned int us){

    us *= SYS_FREQ / 1000000 / 2; // Core Timer updates every 2 ticks

    _CP0_SET_COUNT(0); // Set Core Timer count to 0

    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier

}

void delayMs(unsigned int ms){
    delayUs(1000*ms);
}


void startDelay(){
    _CP0_SET_COUNT(0); // Set Core Timer count to 0
}

void endDelayUs(unsigned int us){
    
    us *= SYS_FREQ / 1000000 / 2; // Core Timer updates every 2 ticks
    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}


void endDelayMs(unsigned int ms){
    endDelayUs(1000*ms);
}

