#include "headers/main.h"
#include <float.h>
void setup(){
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    pinMode(22, INPUT);
    Serial.begin(9600);
}
void loop(){
    delay(20);
    
    _main();    
}
