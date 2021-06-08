//#define SERIAL_TEST
#ifdef SERIAL_TEST

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "io.h"
#include "serial.h"

char s[80];
double a, b;
int main(void){
    initPBCLK();
    initSerial(115200, 4);
    pinMode(LED2, OUTPUT);
    
    delayMs(1000);
    serialWriteString("hola\n");
    while (true){
        if(serialAvailable()){
            serialReadString(s, 1000);
            sscanf(s, "%lf %lf", &a, &b);
            sprintf(s, "a: %lf b: %lf", a, b);
            serialWriteString(s);
            serialWriteChar('\n');
        }


        digitalWrite(LED2, HIGH);
        delayMs(10);
        digitalWrite(LED2, LOW);
        delayMs(10);
    }
    return 0;
}
#endif