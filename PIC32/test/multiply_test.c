#define MULTIPLY_TEST
#ifdef MULTIPLY_TEST
#include <xc.h>
#include "config.h"
#include "io.h"
#include "serial.h"
#include "piston.h"
#include "utils.h"
#include "serial.h"
#include "timer.h"
#include "matlib.h"

timer micros;
char buff[50];
float M1[9][9],M2[9][9],M3[9][9];

unsigned long long time = 0, time_ant;


int main(){
    initPBCLK();
    initSerial(115200, 4); 
    mat A,B,C;

    matInit(&A, 9 , 9);
    matInit(&B, 9 , 9); 
    for(int i = 0; i < 9;  i++)for(int j = 0; j < 9;  j++)
        setMatVal(&A, i, j, i+j), M1[i][j] = i+j;

    for(int i = 0; i < 9;  i++) setMatVal(&B,i,i,1.01), M2[i][i] = 1.01;
    //initTimer(&micros, 2, DIV1, 5);
    //setTimerFrecuency(&micros, 1000000ULL);
    
    //float av = 1.0001, bv = 1.00001, cv = 5;
    matInit(&C, 9, 9);
    int av = 1, bv = 2, cv = 5;
    while(1){
        //time_ant = time;
        serialWriteString( "x\n");
        
        for(int k = 0; k < 1 ; k++)
        
        for(int j = 0; j < 100 ; j++)
        for(int i = 0; i < 100 ; i++){
            //matInit(&C)
            matMult(&C, &A, &B);
            matCopy(&A, &C);
            //matDestruct(&C);
        }
        serialWriteString("y\n");
        delayMs(10);
    }

    return 0;
}


#endif