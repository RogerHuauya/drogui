#define BLINK_TEST
#ifdef BLINK_TEST
#include "_main.h"
#include "tim.h"

void _main(){
    while(1){
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		HAL_Delay(1000);
		
	}
}

#endif
