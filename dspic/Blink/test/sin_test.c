//#define SIN_TEST
#ifdef SIN_TEST

#include "config.h"
#include <libpic30.h>
#include <stdint.h>
#include "serial.h"
#include <math.h>
#define N 1000
#define LED PRTD, 8
serial Serial1;
long long x = 0;
int n;
double y_ser[N];
double u_ser[N];
void initVar(){
    for( int i = 0; i <N; i++){
        u_ser[i] = 5;
        y_ser[i] = 0;
    } 
}
int main(void){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    __delay_ms(1000);
    serialWriteString(&Serial1, "init");
    double Tf=10;
    n = (tf/T)+1;
    long long  t = 0;
    long double  T = 0.01;
    char aux_y[50];
    char aux_u[50];
    initVar();
    while (true){
        int i = 1;
        if( t < tf ){
            y_ser[i] = y_ser[i-1] + 3*u_ser[i];
            sprintf(aux_y, "%lf\n", y_ser[i]);
            serialWriteString(&Serial1, aux_y);
            __delay_ms(1000*T);
        }
        else t = 0;
        i++;
    }
    return 0;
}

#endif