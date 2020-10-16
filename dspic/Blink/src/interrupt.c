#include "interrupt.h"
#include "io.h"


void initInterrupt(int n, int priority){

   switch (n){
      case 0:
         pinMode(PRTD, 0, INPUT);
         INTCON2bits.INT0EP = 0;
         IPC0bits.INT0IP = priority;
         IFS0bits.INT0IF = 0;
         IEC0bits.INT0IE = 1;
         break;

      case 1:
         pinMode(PRTA, 14, INPUT);
         RPINR0bits.INT1R = 30;  // RA14
         INTCON2bits.INT1EP = 1;
         IPC5bits.INT1IP = priority;
         IFS1bits.INT1IF = 0;
         IEC1bits.INT1IE = 1;
         break;

      case 2:
         pinMode(PRTD, 12, INPUT);
         RPINR1bits.INT2R = 0b1010; // RD12
         INTCON2bits.INT2EP = 0;
         IPC7bits.INT2IP = priority;
         IFS1bits.INT2IF = 0;
         IEC1bits.INT2IE = 1;
         break;
      
      case 3:
         pinMode(PRTD, 2, INPUT);
         RPINR1bits.INT3R = 0;
         INTCON2bits.INT3EP = 0;
         IPC13bits.INT3IP = priority;
         IFS3bits.INT3IF = 0;
         IEC3bits.INT3IE = 1;
         break;

      case 4:
         pinMode(PRTD, 3, INPUT);
         RPINR2bits.INT4R = 0;
         INTCON2bits.INT4EP = 0;
         IPC13bits.INT4IP = priority;
         IFS3bits.INT4IF = 0;
         IEC3bits.INT4IE = 1;
         break;
   }
}

void clearIntFlag(int n){
   switch(n){
      case 0: IFS0bits.INT0IF = 0; break;
      case 1: IFS1bits.INT1IF = 0; break;
      case 2: IFS1bits.INT2IF = 0; break;
      case 3: IFS3bits.INT3IF = 0; break;
      case 4: IFS3bits.INT4IF = 0; break;
   }
}
