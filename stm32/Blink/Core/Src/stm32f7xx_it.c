/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "task.h"
#include "serial.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

double distance = 0;
uint32_t time_ant;

char buffer[50]="";
int write_adress = -1, read_adress = -1;
int index_i2c = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c4;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */  
  
  if(__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_1)){
    bool echo_state = HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET;
    if(echo_state) time_ant = TIME;
    else distance = 0.0003432*(TIME - time_ant)/2.0;
    //LD2_GPIO_Port -> ODR ^= LD2_Pin; // toggle LD2 LED
  } 
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
	/* USER CODE BEGIN USART2_IRQn 0 */
	unsigned long isr = USART2->ISR;
	if(isr & USART_ISR_RXNE){

		rcv_buff[rcv_head++] = USART2->RDR;
        rcv_head %= SER_BUFF_SZ;
        rcv_tail += (rcv_head == rcv_tail);
        rcv_tail %= SER_BUFF_SZ;
	
	}
	else if(isr & USART_ISR_TXE){
    
		if(snd_head != snd_tail){
			USART2->TDR = snd_buff[snd_tail++];
			snd_tail %= SER_BUFF_SZ;
		}
		if(snd_head == snd_tail){
			USART2->CR1  &= ~(USART_CR1_TXEIE);
		}
	}
	/* USER CODE END USART2_IRQn 0 */
	// HAL_UART_IRQHandler(&huart2);
	/* USER CODE BEGIN USART2_IRQn 1 */

	/* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles I2C4 event interrupt.
  */
void I2C4_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C4_EV_IRQn 0 */
  sprintf(buffer, "%x \t %x \t %x \n", write_adress, read_adress, index_i2c);
  //HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), 100);

  unsigned long isr = I2C4->ISR;

  if ( isr & I2C_ISR_TXIS ) 
  {
    I2C4->TXDR = i2cReg[read_adress][ index_i2c++ ];
    if(index_i2c == 4) index_i2c = 0;
  }
  else if ( isr & I2C_ISR_RXNE ){

    //HAL_UART_Transmit(&huart2, (uint8_t*) "RX\n", 4, 100);
    uint8_t a = (I2C4->RXDR);
    if(write_adress == -1){
        if(a & 1){
          write_adress = a;
        }
        else{
          read_adress = a;
        }
    }
    else{
      i2cReg[write_adress^1][ index_i2c++ ] = a;
    }
    if(index_i2c == 4) write_adress = -1, index_i2c = 0;
    
    //printf(buffer, "Rece %d %d\n", index1, index2);
    //HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), 100);
  }
  else if ( isr & I2C_ISR_STOPF )
  {
    if(index_i2c != 0) write_adress = read_adress = -1, index_i2c = 0;
    //HAL_UART_Transmit(&huart2, (uint8_t*) "STOP\n", 6, 100);
    I2C4->ISR |= I2C_ISR_TXE;
    I2C4->ICR = I2C_ICR_STOPCF;
  }
  else if ( isr & I2C_ISR_NACKF )
  {
    //HAL_UART_Transmit(&huart2, (uint8_t*) "NACK\n", 6, 100);
    I2C4->ICR = I2C_ICR_NACKCF;
  }
  else if ( isr & I2C_ISR_ADDR )
  {
    //HAL_UART_Transmit(&huart2, (uint8_t*) "ADDR\n", 6, 100);
    /* not required, the addr match interrupt is not enabled */
    I2C4->ICR = I2C_ICR_ADDRCF;
  }
 
  /* if at any time the addr match is set, clear the flag */
  /* not sure, whether this is required */
  if ( isr & I2C_ISR_ADDR )
  {
    I2C4->ICR = I2C_ICR_ADDRCF;
  }
   
  /* USER CODE END I2C4_EV_IRQn 0 */
  //HAL_I2C_EV_IRQHandler(&hi2c4);
  /* USER CODE BEGIN I2C4_EV_IRQn 1 */

  /* USER CODE END I2C4_EV_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
