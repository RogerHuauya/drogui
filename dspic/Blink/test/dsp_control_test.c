//#define DSP_CONTROL_TEST
#ifdef DSP_CONTROL_TEST

#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <dsp.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "serial.h"
serial Serial1;
double kp = 9, ki = 0.01, kd = 0;
double T = 0.001;
int tf=4;
int K = 100;
int n;
int ref = 10;
double  t = 0;
double y_ant = 0, u_ant = 0;
double y = 0, u = 0;
tPID fooPID;
fractional abcCoefficient[3];// __attribute__ ((section (".xbss, bss, xmemory")));
fractional controlHistory[3];// __attribute__ ((section (".ybss, bss, ymemory")));
fractional kCoeffs[] = {0,0,0};

double plant(){
    double y_plant;
    y_plant = (K/(K+T))*y_ant + (K*T/(K+T))*(u);
    u_ant = u, y_ant = y_plant;
    return y_plant;
}

void InitPID( double kp, double ki, double kd){
    fooPID.abcCoefficients = &abcCoefficient[0];    /*Set up pointer to derived coefficients */
    fooPID.controlHistory = &controlHistory[0];     /*Set up pointer to controller history samples */
    PIDInit(&fooPID);                               
	kCoeffs[0] = Q15(kp);
	kCoeffs[1] = Q15(ki);
	kCoeffs[2] = Q15(kd);
    PIDCoeffCalc(&kCoeffs[0], &fooPID); 
}
int main(void) {
    while(1){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    __delay_ms(1000);
    serialWriteString(&Serial1, "init");
    char aux_y[50];
    char aux_u[50];
    InitPID(kp,ki,kd);
        while (t < tf){
            fooPID.controlReference = Q15(ref) ;
            fooPID.measuredOutput = Q15(plant());
            PID(&fooPID);
            u = Fract2Float(fooPID.controlOutput);  
            //printf("U:%.3lf\tY:%.3lf\tTime:%.3lf\n",u, y, t);
            t+=T;
            sprintf(aux_y, "%lf\n", y);
            serialWriteString(&Serial1, aux_y);
            __delay_ms(1000*T);
        }
        t=0;

    }    
	return 0;
}

#endif