#define MOTOR_TEST
#ifdef MOTOR_TEST


#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "debugTasks.h"
#include "task.h"
#include "serial.h"
#include "pwm.h"

pwm m1_test, m2_test, m3_test, m4_test;
void _main(){
    /**
     *
     * 1 4
     * 2 1
     * 3 3
     * 4 2
     *
     */
    initPwm(&m1_test, &htim3, TIM_CHANNEL_1, &(htim3.Instance->CCR1));
    initPwm(&m2_test, &htim3, TIM_CHANNEL_2, &(htim3.Instance->CCR2));
    initPwm(&m3_test, &htim4, TIM_CHANNEL_3, &(htim4.Instance->CCR3));
    initPwm(&m4_test, &htim4, TIM_CHANNEL_4, &(htim4.Instance->CCR4));

    while(true){
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

        //for(int i = 0; i < 5; i++ ) {
			setPwm(&m1_test, 0); //2
			setPwm(&m2_test, 30); //4
			setPwm(&m3_test, 60); // 3
			setPwm(&m4_test, 90);//1 
			
			HAL_Delay(2000); 
			//setPwm(&m1_test, 0);
		//}
		//setPwm(&m2_test, 20); HAL_Delay(2000); setPwm(&m2_test, 0);
        //setPwm(&m3_test, 20); HAL_Delay(2000); setPwm(&m3_test, 0);
        //setPwm(&m4_test, 20); HAL_Delay(2000); setPwm(&m4_test, 0);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_Delay(1000);
    }
}
#endif
