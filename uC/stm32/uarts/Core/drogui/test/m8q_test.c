//#define M8Q_TEST
#ifdef M8Q_TEST
#include "_main.h"
#include "tim.h"
#include "macros.h"
#include "task.h"
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
	changeBaudrate(SER_DBG,2000000);
    serialPrint(SER_DBG, "Init\n");
    
    SENSOR_STATUS ret_init = initM8Q(&myGPSt, SER_GPS);
    
	serialPrintf(SER_DBG, "Ret init %d\n", ret_init);

    while(1){
        SENSOR_STATUS ret = readLatLon(&myGPSt);
        if(ret == OK)
            serialPrintf(SER_DBG, "lat: %d, lon: %d\n", myGPSt.latitude, myGPSt.longitud);
        else if(ret != NO_DATA)
            serialPrintf(SER_DBG, "Error: %d\n", ret);

    }
} 

#endif
