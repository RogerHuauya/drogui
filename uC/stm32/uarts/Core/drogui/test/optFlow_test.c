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
    serialsBegin();
    initOptFlow(&of,&serial2);
    
    while(1){

        int ret = readFlowRange(&of);
           
        if(ret == OPT_VEL || ret == OPT_RNG){
            serialPrintf(&serial3, "%d %d %d\n", of.dis, of.vel_x, of.vel_y);
        }
        else if( ret != OPT_NO_DATA){
            serialPrintf(&serial3, "Error %d %X\n", ret, of.rcv_pack.type);
        }
        //serialPrint(&serial3, "Hola mundo\n");
        HAL_Delay(1);
    }
    
}

#endif