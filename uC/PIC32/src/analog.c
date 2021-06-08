#ifdef NOSE
#include <xc.h>
#include "analog.h"


void initAnalog(){
    AD1CAL1 = 0xB3341210;
    AD1CAL2 = 0x01FFA769;
    AD1CAL3 = 0x0BBBBBB8;
    AD1CAL4 = 0x000004AC;
    AD1CAL5 = 0x02028002;


    ADCCON1bits.FORM = 0;   
    ADCCON1bits.SIDL = 0; 
    ADCCON1bits.SSRC = 0;   

    ADCCON2bits.VCFG = 0;   
    ADCCON2bits.CSCNA = 0;  
    ADCCON2bits.SMPI = 0;  
    ADCCON2bits.BUFM = 0;   
    ADCCON2bits.ALTS = 0;   
    
    ADCCON3bits.ADRC = 0;   
    ADCCON3bits.ADCS = 0;   
 
    ADCCON1bits.ON = 1;   
    ADCPCFGbits.PCFG5 = 0;
}

uint16_t analogRead(int pin){
    ADCCHSbits.CH0NA = 0;
    ADCCHSbits.CH0SA = pin;
    ADCCON1bits.SAMP = 1;
    delayUs(10);
    ADCCON1bits.SAMP = 0;
    while(!ADCCON1bits.DONE){}
    ADCCON1bits.DONE = 0;
    return ADC1BUF0;
}

#endif