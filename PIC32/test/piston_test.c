//#define PISTON_TEST
#ifdef PISTON_TEST

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "io.h"
#include "serial.h"
#include "piston.h"
#include "utils.h"
#include "sensors.h"


#define AUX PRTD, 1
char s[80];
int pos_d = 0, vel_d;
timer my_timer;
piston my_piston;
press piston_press;
press lung_press;
int k;

int main(void){
    
    initPBCLK();
    initSerial(115200, 4);
    delayMs(100);
    initPress(&piston_press, I2C1);
    initPress(&lung_press, I2C2);

    pinMode(LED1, OUTPUT);
    pinMode(BOB_INS, OUTPUT);
    pinMode(BOB_FIL, OUTPUT);
    pinMode(BOB_ESP, OUTPUT);
    pinMode(AUX, INPUT);
    
    digitalWrite(BOB_FIL, LOW);
    digitalWrite(BOB_INS, LOW);
    digitalWrite(BOB_ESP, LOW);


    setPiston(&my_piston);
    initPistonStepper(PRTC, 1, PRTC , 4);

    // CTX C1 -> STP
    // CRX C4 -> DIR
    // DIR, STP, GND

    initPistonVelTimer(DIV256);
    initPistonAccelTimer(DIV256, 2000);
    
    digitalWrite(BOB_FIL,LOW);
    moveHome(&my_piston);
    digitalWrite(BOB_FIL,HIGH);

    initTimer(&my_timer, 4, 5, DIV256);
    setTimerFrecuency(&my_timer, 100);

    pos_d = 0;
    vel_d = 100;
    k = 0;    
    delayMs(1000);
    


    while (true){
        
       
        if(serialAvailable()){
            char c = serialReadChar();
            if(c == 'p')
                pos_d = serialParseInt();
                if(pos_d == -1){ 
                    stopPiston(&my_piston);
                    moveHome(&my_piston);
                }
                pos_d = min(max(pos_d, 0), MAXLEN);
            if(c == 'v')
                vel_d = serialParseInt();
        }
        k = min(110,abs(1.0*pos_d - getPos(&my_piston))*10);
        setVelPiston(&my_piston, max(min(k*(pos_d - getPos(&my_piston)) , vel_d), -vel_d));
        delayUs(1);
    }

    return 0;
}


void timerInterrupt(4, 5){
    if(digitalRead(AUX))
        digitalWrite(LED1, HIGH),
        digitalWrite(BOB_INS, LOW),
        digitalWrite(BOB_FIL, HIGH),
        digitalWrite(BOB_ESP, HIGH);
    else
        digitalWrite(LED1, LOW),
        digitalWrite(BOB_INS, HIGH),
        digitalWrite(BOB_FIL, LOW),
        digitalWrite(BOB_ESP, LOW);

    readPress(&lung_press);
    readPress(&piston_press);
    sprintf(s,"%4d,%4d,\t %4.3lf ,\t %4.3lf\n", pos_d, vel_d, lung_press.val, piston_press.val);
    serialWriteString(s); 
    clearTimerFlag(&my_timer);
}


#endif
