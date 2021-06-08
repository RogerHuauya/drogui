//#define OPTFLOW_TEST
#ifdef OPTFLOW_TEST

#include "_main.h"
#include "tim.h"
#include "task.h"
#include "serial.h"
#include "opticalFlow.h"

optFlow of;

char auxbuff[50];
void _main(){
    initOptFlow(&of);
    while(1){
        /*
        while(serialAvailable()){
            serialPrintf("%c\n", serialRead());
        }
        
        else
            serialPrint("no data\n");*/


        int ret = readFlowRange(&of);
           
        if(ret == OPT_VEL || ret == OPT_RNG){
            serialPrintf("%d %d %d\n", of.dis, of.vel_x, of.vel_y);
        }
        else if( ret != OPT_NO_DATA){
            serialPrintf("Error %d %X\n", ret, of.rcv_pack.type);
        }

        HAL_Delay(1);
    }
    
}

#endif