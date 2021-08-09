//#define FFT_TEST
#ifdef FFT_TEST

#include "_main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "_freertos.h"
#include "sensorsTasks.h"

arm_rfft_fast_instance_f32 fft;

float fft_aux[] = {0,3.004486e+01,2.002994e+01,0,1.001497e+01,2.002994e+01,0,1.001497e+01,2.002994e+01,0,-1.001495e+01,-2.002994e+01,3.004491e+01,1.001497e+01,1.001497e+01,6.008982e+01,2.002991e+01,2.002994e+01,5.007485e+01,0,-6.008982e+01,3.004491e+01,3.004491e+01,2.002994e+01,2.002994e+01,1.001497e+01,3.004486e+01,2.002991e+01,4.005994e+01,1.001497e+01,0,5.007478e+01,3.004491e+01,2.002994e+01,0,2.002994e+01,0,0,0,1.001497e+01,5.007485e+01,1.001497e+01,5.007478e+01,3.004491e+01,6.008982e+01,3.004495e+01,3.004486e+01,4.005988e+01,-2.002991e+01,-1.001495e+01,0,3.004491e+01,1.001497e+01,0,0,0,1.001497e+01,2.002994e+01,3.004491e+01,1.001495e+01,2.002994e+01,1.001495e+01,-5.007485e+01,1.001497e+01};
float fft_ans[64];
float fft_arr[64];


osThreadId_t debugHandle;
const osThreadAttr_t debugAttributes = {    .name = "debugTask", \
	.stack_size = 256 * 4, \
		.priority = (osPriority_t) osPriorityNormal};


osThreadId_t blinkHandle;
const osThreadAttr_t blinkAttributes = {    .name = "blinkTask",\
	.stack_size = 128 * 4,\
		.priority = (osPriority_t) osPriorityNormal};

osThreadId_t fftHandle;
const osThreadAttr_t fftAttributes = {    .name = "fftTask",\
	.stack_size = 128 * 4,\
		.priority = (osPriority_t) osPriorityNormal};

void blinkTask(void *argument){

	for(;;){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		osDelay(1000);
	}
}

void fftTask(void *argument){

	for(;;){
		for(int i = 0; i< 64 ; i ++)
			fft_arr[i] = fft_aux[i];

		arm_rfft_fast_f32(&fft, fft_arr, fft_ans, 0);

		for(int i = 0; i< 64 ; i += 2)
			fft_ans[i/2] = sqrt(fft_ans[i]*fft_ans[i] + fft_ans[i+1]*fft_ans[i+1]);

		for(int i = 0; i < 32 ; i ++)
			fft_ans[i] = (i > 0 ? 2 : 1) * fft_ans[i] / 64;
		osDelay(1000);
	}
}



char buff[50] = "hola";
void debugTask(void *argument){

	for(;;){

		sprintf(buff, "fft_ans = [");
		HAL_UART_Transmit(&huart1, (uint8_t*) buff, strlen(buff), 100);
		for(int i = 0; i < 32 ; i ++){

			sprintf(buff, "%f\t", fft_ans[i]);
			HAL_UART_Transmit(&huart1, (uint8_t*) buff, strlen(buff), 100);
		}
		sprintf(buff, "];\n");
		HAL_UART_Transmit(&huart1, (uint8_t*) buff, strlen(buff), 100);
		osDelay(1000);
	}
}

void initSystem(){
	arm_rfft_fast_init_f32(&fft, 64);

	blinkHandle = osThreadNew(blinkTask, NULL, &blinkAttributes);
	debugHandle = osThreadNew(debugTask, NULL, &debugAttributes);
	fftHandle = osThreadNew(fftTask, NULL, &fftAttributes);
}

#endif
