#include <xc.h>
#include "analog.h"

void initAnalog(){
    AD1CON1bits.FORM = 0;
    AD1CON1bits.ADSIDL = 0;
    AD1CON1bits.AD12B = 1;
    AD1CON1bits.SSRC = 0;
    
    AD1CON2bits.VCFG = 0;
    AD1CON2bits.CSCNA = 1;
    AD1CON2bits.SMPI = 15;
    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0;
    
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.ADCS = 0;
    
    AD1CON4bits.DMABL = 3;
    AD1CON1bits.ADON = 1;
    
    AD1PCFGL = 0;
}

uint16_t analogRead(int pin){
    AD1CHS0bits.CH0NA = 1;
    AD1CHS0bits.CH0SA = pin;
    AD1CON1bits.SAMP = 1;
    __delay_us(10);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE){}
    AD1CON1bits.DONE = 0;
    return ADC1BUF0;
}
