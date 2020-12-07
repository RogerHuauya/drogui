//#define PISTON_TEST2
#ifdef PISTON_TEST2

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "io.h"
#include "serial.h"
#include "piston.h"
#include "utils.h"
#include "sensors.h"


#define AUX PRTD, 1
char s[80], read_buffer[80];
piston my_piston;
int k;

int main(void){
    
    initPBCLK();
    initSerial(115200, 4);
    delayMs(100);


    setPiston(&my_piston);
    initPistonStepper(PRTC, 1, PRTC , 4);
    initPistonVelTimer(DIV256);
    initPistonAccelTimer(DIV256, 2000);
    k = 0;
    while (true){
        if(serialAvailable()){
            serialReadString(read_buffer, 1000);
            sscanf(read_buffer, "%d", &k);
            sprintf(s, "k: ")
        }
        setVelPiston(&my_piston, k);
        delayUs(1);
    }

    return 0;
}


#endif
