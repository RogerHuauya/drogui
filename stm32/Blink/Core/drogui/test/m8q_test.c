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
#include "registerMap.h"

m8q myGPS;
char auxbuff[50];
int flag = 0, head = 0;
uint8_t buff[1000];


void gpsTask(){


    //serialPrint("nose\n");
    int ret = readLatLon(&myGPS); 
    /*if(ret == GPS_OK)
        serialPrintf("Latitude: %d, Longitud: %d\n", myGPS.latitude, myGPS.longitud);
    else if(ret == WRG_CLS_ID)
        serialPrintf("Class: %d, ID: %d\n", myGPS.rcv_pack.cls, myGPS.rcv_pack.id);
    else if(ret == WRG_CHKSUM)
        serialPrint("Wrong checksum\n");
    else if(ret == TIMEOUT)
        serialPrint("Timeout\n");
    serialPrintf("return: %d\n", ret);*/
    setReg(GPS_X, myGPS.latitude/10000.0);
    setReg(GPS_Y, myGPS.longitud/10000.0);
    setReg(GPS_STATE, ret);
}




void _main(){
    
    HAL_Delay(1000);
    cfgM8QPort(&(myGPS.snd_pack), defaultCfgPort);
    
    HAL_Delay(100);
    changeBaudrate(460800);
    HAL_Delay(100);

    cfgM8QRate(&(myGPS.snd_pack), defaultCfgRate);
    readM8Q(&(myGPS.rcv_pack), 10000);

    cfgM8QMsg(&(myGPS.snd_pack), defaultCfgMsg); 
    readM8Q(&(myGPS.rcv_pack), 10000);

    HAL_Delay(100);
    
    addTask(&gpsTask, 1000000, 1);
    initRTOS();


    /*while(1){
        HAL_Delay(1000);

        
        
        serialPrint("hola mundo\n");
        if(serialAvailable()){
            if(readM8Q(&(myGPS.rcv_pack))){
                int latitude = 0, longitud = 0;
                for(int i = 0 ; i < 4 ; i++) 
                    latitude = (latitude << 8) | (myGPS.rcv_pack.payload[27-i]);
                for(int i = 0 ; i < 4 ; i++) 
                    longitud = (longitud << 8) | (myGPS.rcv_pack.payload[31-i]);
                
                Serial.print(latitude);
                Serial.print("\t");
                Serial.print(longitud);
                printPacket(&(myGPS.rcv_pack));
            }
        }
    }*/
} 

#endif