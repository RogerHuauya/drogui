//#define M8Q_TEST
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
	serialsBegin();
	serialPrint(&serial3, "Init\n");
    SENSOR_STATUS ret_init = initM8Q(&myGPSt, &serial2);
    
	serialPrintf(&serial3, "Ret init %d\n", ret_init);

    while(1){
        SENSOR_STATUS ret = readLatLon(&myGPSt);
        if(ret == OK)
            serialPrintf(&serial3, "lat: %d, lon: %d\n", myGPSt.latitude, myGPSt.longitud);
        else if(ret != NO_DATA)
            serialPrintf(&serial3, "Error: %d\n", ret);

    }
} 

#endif
