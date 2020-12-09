//#define PERIPHERAL_TEST
#ifdef PERIPHERAL_TEST
#include "config.h"
#include "utils.h"
#include "press_reg.h"
#include "io.h"
#include "serial.h"

#define AUX PRTD, 1
press_reg pr;
press piston_press;
char s[50];

int main(){
    initPBCLK();
    initSerial(115200, 4);
    pinMode(AUX, INPUT);
    pinMode(BOB_INS, OUTPUT);
    pinMode(BOB_FIL, OUTPUT);

    digitalWrite(BOB_FIL, LOW);
    digitalWrite(BOB_INS, HIGH);

    initPressReg(&pr, PRTD, 15, PRTD, 9);
    initPress(&piston_press, I2C1);
    while(1){
        readPress(&piston_press);
        digitalWrite(BOB_FIL, !digitalRead(AUX));

        sprintf(s, "press: %5.2lf\n", piston_press.val);
        serialWriteString(s);
        delayMs(10);
    }
}

#endif