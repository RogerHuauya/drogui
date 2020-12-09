#include "interrupt.h"
#include "io.h"


void initInterrupts(int n, int priority){

   switch (n){
      case 0:
         pinMode(PRTD, 0, INPUT);
         INTCONbits.INT0EP = 0;
         IPC0bits.INT0IP = priority;
         IFS0bits.INT0IF = 0;
         IEC0bits.INT0IE = 1;
         break;

      case 1:
         pinMode(PRTD, 1, INPUT);
         INT1Rbits.INT1R = 0;          // RD1
         INTCONbits.INT1EP = 0;
         IPC2bits.INT1IP = priority;
         IFS0bits.INT1IF = 0;
         IEC0bits.INT1IE = 1;
         break;

      case 2:
         pinMode(PRTD, 12, INPUT);
         INT2Rbits.INT2R = 0b1010; // RD12
         INTCONbits.INT2EP = 0;
         IPC3bits.INT2IP = priority;
         IFS0bits.INT2IF = 0;
         IEC0bits.INT2IE = 1;
         break;
      
      case 3:
         pinMode(PRTD, 2, INPUT);
         INT3Rbits.INT3R = 0;
         INTCONbits.INT3EP = 0;
         IPC4bits.INT3IP = priority;
         IFS0bits.INT3IF = 0;
         IEC0bits.INT3IE = 1;
         break;

      case 4:
         pinMode(PRTD, 3, INPUT);
         INT4Rbits.INT4R = 0;
         INTCONbits.INT4EP = 0;
         IPC5bits.INT4IP = priority;
         IFS0bits.INT4IF = 0;
         IEC0bits.INT4IE = 1;
         break;
   }
}

void clearIntFlag(int n, int priority){
   switch(n){
      case 0: IFS0bits.INT0IF = 0; break;
      case 1: IFS0bits.INT1IF = 0; break;
      case 2: IFS0bits.INT2IF = 0; break;
      case 3: IFS0bits.INT3IF = 0; break;
      case 4: IFS0bits.INT4IF = 0; break;
   }
}
