//#define TERARANGER
#ifdef TERARANGER
#include "_main.h"
#include "serial.h"
#include "teraRanger.h"

tRanger myTera;

void _main(){

    serialsBegin();
    initTeraRanger(&myTera, &serial2);
    while(1){
        SENSOR_STATUS ret = readTeraRange(&myTera);
        if(ret == OK){
            serialPrintf(&serial3, "Dis = %d\n", myTera.distance);

        }
        else if(ret != NO_DATA ){
            serialPrintf(&serial3, "Error = %d\n", ret);
        }


    }
    
}

#endif