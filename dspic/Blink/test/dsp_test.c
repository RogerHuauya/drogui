//#define DSP_TEST
#ifdef DSP_TEST

#include <xc.h>
#include "config.h" 
#include "serial.h"
#include "matlib.h"
#include <libpic30.h>
mat mat1;
mat mat2;
mat mat3;
serial Serial1;

char s[80];

int main(){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    __delay_ms(1000);

    serialWriteString(&Serial1,"hola1");
    setMatVal(&mat1, 0, 0, 0.3);
    setMatVal(&mat1, 0, 1, 0.2);
    setMatVal(&mat1, 1, 0, 0.1);
    setMatVal(&mat1, 1, 1, 0.8);

    setMatVal(&mat2, 0, 0, -0.1);
    setMatVal(&mat2, 0, 1, 0.4);
    setMatVal(&mat2, 1, 0, 0.2);
    setMatVal(&mat2, 1, 1, 0.9);

    serialWriteString(&Serial1,"hola1");
    matMult(&mat3, &mat1, &mat2);
    serialWriteString(&Serial1,"hola1");

    sprintf(s, "%.2lf\t%.2lf\n%.2lf\t%.2lf\n\n",   getMatVal(&mat3,0,0), getMatVal(&mat3,0,1),\
                                                            getMatVal(&mat3,1,0), getMatVal(&mat3,1,1));

    while(1){
        serialWriteString(&Serial1, s);
        __delay_ms(1000);
    }
    
}

#endif