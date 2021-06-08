//#define DSP_TEST
#ifdef DSP_TEST

#include <xc.h>
#include "config.h" 
#include "serial.h"
#include "matlib.h"
#include <libpic30.h>
#include <dsp.h>


mat mat1;
mat mat2;
mat mat3;
serial Serial1;

fractional m1[2][2];
fractional m2[2][1];
fractional m3[2][1];

char s[80];

int main(){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    __delay_ms(1000);
/*
    matInit(&mat1, 2, 2);
    matInit(&mat2, 2, 1);
    matInit(&mat3, 2, 1);

    setMatVal(&mat1, 0, 0, 0.3);
    setMatVal(&mat1, 0, 1, 0.2);
    setMatVal(&mat1, 1, 0, 0.1);
    setMatVal(&mat1, 1, 1, 0.8);
*/
    m1[0][0] = Float2Fract(0.3);
    m1[0][1] = Float2Fract(0.2);
    m1[1][0] = Float2Fract(0.1);
    m1[1][1] = Float2Fract(0.8);
    
  /*  
    setMatVal(&mat2, 0, 0, -0.1);
    //setMatVal(&mat2, 0, 1, 0.4);
    setMatVal(&mat2, 1, 0, 0.2);
    //setMatVal(&mat2, 1, 1, 0.9);
*/
    m2[0][0] = Float2Fract(-0.1);
    m2[1][0] = Float2Fract(0.2);



    //MatrixMultiply(2, 2, 1, &m3[0][0], &m1[0][0], &m2[0][0]);

    MatrixScale(2,1,&m2[0][0], &m2[0][0], Float2Fract(0.8));

    //matMult(&mat3, &mat1, &mat2);
    while(1){

        for(int i = 0 ; i < 2 ; i++){
            for(int j = 0 ; j < 1 ; j++)
                sprintf(s, "%.2lf\t", Fract2Float(m2[i][j])), serialWriteString(&Serial1, s);
            
            serialWriteString(&Serial1,"\n");
        }
        
        serialWriteString(&Serial1,"\n");

        __delay_ms(1000);
    }

}

#endif