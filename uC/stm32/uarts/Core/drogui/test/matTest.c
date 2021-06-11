
//#define MAT_TEST
#ifdef MAT_TEST

#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "debugTasks.h"
#include "task.h"
#include "serial.h"
#include "usart.h"
#include "macros.h"
#include "matlib.h"
char auxbuff[50];

mat a, b, c, d, e;

void _main(){
    serialsBegin();
    changeBaudrate(SER_DBG, 2000000); 
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    
    matInit(&a, 3, 3);
	matInit(&b, 3, 3);
	matInit(&c, 3, 3);
    matInit(&d, 3, 1);
    matInit(&e, 3, 1);

	setMatVal(&a, 0, 0, 2.5);
	setMatVal(&a, 0, 1, 1.47);
	setMatVal(&a, 0, 2, 1.74);
	setMatVal(&a, 1, 0, -1.7);
	setMatVal(&a, 1, 1, 0.7);
	setMatVal(&a, 1, 2, 4.7);
	setMatVal(&a, 2, 0, 4.7);
	setMatVal(&a, 2, 1, 7.1);
	setMatVal(&a, 2, 2, 2.7);

	setMatVal(&b, 0, 0, 5.7);
	setMatVal(&b, 0, 1, 4.9);
	setMatVal(&b, 0, 1, 4.9);
	setMatVal(&b, 1, 0, 4.7);
	setMatVal(&b, 1, 1, 2.5);
	setMatVal(&b, 1, 2, 4.5);
	setMatVal(&b, 2, 0, 4.7);
	setMatVal(&b, 2, 1, 2.1);
	setMatVal(&b, 2, 2, 4.7);

	setMatVal(&d, 0, 0, 1.4);
	setMatVal(&d, 1, 0, 6.3);
	setMatVal(&d, 2, 0, 1.3);

    matAdd(&c, &a, &b);
	for(int i = 0; i < c.row; i++){
		for(int j = 0; j < c.col; j++){
		    serialPrintf(SER_DBG, "%.3f ", getMatVal(&c, i, j));
		}
		serialPrint(SER_DBG, "\n");
    }
	serialPrint(SER_DBG, "\n");
    
	matSubs(&c, &a, &b);
	for(int i = 0; i < c.row; i++){
		for(int j = 0; j < c.col; j++){
		    serialPrintf(SER_DBG, "%.3f ", getMatVal(&c, i, j));
		}
		serialPrint(SER_DBG, "\n");
    }
	serialPrint(SER_DBG, "\n");

    matMult(&c, &a, &b);
	for(int i = 0; i < c.row; i++){
		for(int j = 0; j < c.col; j++){
		    serialPrintf(SER_DBG, "%.3f ", getMatVal(&c, i, j));
		}
		serialPrint(SER_DBG, "\n");
    }
	serialPrint(SER_DBG, "\n");
	

    matTrans(&c, &a);
	for(int i = 0; i < c.row; i++){
		for(int j = 0; j < c.col; j++){
		    serialPrintf(SER_DBG, "%.3f ", getMatVal(&c, i, j));
		}
		serialPrint(SER_DBG, "\n");
    }
	serialPrint(SER_DBG, "\n");
	
	
    for(int i = 0; i < c.row; i++){
		for(int j = 0; j < c.col; j++){
		    serialPrintf(SER_DBG, "%.3f ", getMatVal(&c, i, j));
		}
		serialPrint(SER_DBG, "\n");
    }
	serialPrint(SER_DBG, "\n");
	
    matInv(&c, &a);
	for(int i = 0; i < c.row; i++){
		for(int j = 0; j < c.col; j++){
		    serialPrintf(SER_DBG, "%.3f ", getMatVal(&c, i, j));
		}
		serialPrint(SER_DBG, "\n");
    }
	serialPrint(SER_DBG, "\n");
	
    gaussElimination3x3(&a, &d, &e);
	for(int i = 0; i < e.row; i++)
		serialPrintf(SER_DBG, "%.3f ", getMatVal(&e, i, 0));
	serialPrint(SER_DBG, "\n");

    serialPrint(SER_DBG, "Start System\n");
    

    //sprintf(auxbuff, "%d\n", __FPU_USED);
    //HAL_UART_Transmit(&huart2, (uint8_t*) auxbuff, strlen(auxbuff), 100);
    HAL_Delay(10000);
    
}

#endif
