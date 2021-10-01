#define MOTOR_TEST
#ifdef MOTOR_TEST
#include "..\headers\main.h"
#include "..\headers\pwm.h"

static pwm m1, m2, m3, m4;
void initializeSystem(){

    initOneshot125(&m1, 2);
    initOneshot125(&m2, 3);
    initOneshot125(&m3, 4);
    initOneshot125(&m4, 5);
}

int main(){
    initializeSystem();
    delay(1000);

    while(1){
        int pot = analogRead(24);
        
        Serial.println(100*pot/1023);
        setPwmDuty(&m1, 100*pot/1023);
        digitalWrite(13, HIGH);
        delay(100);




        //setPwmDuty(&m1, min(max(0,0),100));
        //digitalWrite(13, HIGH);
        //delay(10000);


        //setPwmDuty(&m1, min(max(10,0),100));
        //digitalWrite(13, HIGH);
        //delay(1000);
        //
        //setPwmDuty(&m1, min(max(30,0),100));
        //digitalWrite(13, LOW);
        //delay(1000);
        //
        //setPwmDuty(&m1, min(max(60,0),100));
        //digitalWrite(13, HIGH);
        //delay(1000);
        //
        //setPwmDuty(&m1, min(max(80,0),100));
        //digitalWrite(13, LOW);
        //delay(1000);
        
    }
}
#endif
