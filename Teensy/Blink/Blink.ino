#include "headers/main.h"
void setup(){
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    pinMode(22, INPUT);
    Serial.begin(9600);
}
void loop(){
    delay(20);
    Serial.println("hola");
    _main();    
}
