#define M8Q_TEST
#ifdef M8Q_TEST
#include "_main.h"
#include "tim.h"
#include "task.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "M8Q.h"

m8q myGPS;
char auxbuff[50];
int flag = 0, head = 0;
uint8_t buff[1000];

void _main(){

    cfgM8QPort(&(myGPS.snd_pack), defaultCfgPort);
    HAL_Delay(100);
    //Serial1.begin(460800);

    cfgM8QRate(&(myGPS.snd_pack), defaultCfgRate);
    readM8Q(&(myGPS.rcv_pack));
    cfgM8QMsg(&(myGPS.snd_pack), defaultCfgMsg);
    readM8Q(&(myGPS.rcv_pack));
    HAL_Delay(100);

    while(1){
        if(serialAvailable()){
            if(readM8Q(&(myGPS.rcv_pack))){
                int latitude = 0, longitud = 0;
                for(int i = 0 ; i < 4 ; i++) 
                    latitude = (latitude << 8) | (myGPS.rcv_pack.payload[27-i]);
                for(int i = 0 ; i < 4 ; i++) 
                    longitud = (longitud << 8) | (myGPS.rcv_pack.payload[31-i]);
                /*
                Serial.print(latitude);
                Serial.print("\t");
                Serial.print(longitud);*/
                
            }
        }
    }
} 

#endif