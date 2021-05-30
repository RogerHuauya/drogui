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

m8q myGPSt;
char auxbuff[50];
int flag = 0, head = 0, cnt = 0;
uint8_t buff[1000];
int pos_x, pos_y;



void _main(){
    HAL_Delay(1000);
    initM8Q(&myGPSt, &serial2);

    while(1){
        int ret = readLatLon(&myGPSt);
        if(ret == GPS_OK)
            serialPrintf(&serial3, "lat: %d, lon: %d\n", myGPSt.latitude, myGPSt.longitud);
        else if(ret != NO_DATA)
            serialPrintf(&serial3, "Error: %d\n", ret);

    }
} 

#endif