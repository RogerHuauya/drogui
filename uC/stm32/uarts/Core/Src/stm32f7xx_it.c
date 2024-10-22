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
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
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
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  unsigned long isr = USART2->ISR;
	if(isr & USART_ISR_RXNE){

    serial2.rcv_buff[serial2.rcv_head++] = USART2->RDR;
    serial2.rcv_head %= SER_BUFF_SZ;
    serial2.rcv_tail += (serial2.rcv_head == serial2.rcv_tail);
    serial2.rcv_tail %= SER_BUFF_SZ;
	
	}
	else if(isr & USART_ISR_TXE){
    
		if(serial2.snd_head != serial2.snd_tail){
			USART2->TDR = serial2.snd_buff[serial2.snd_tail++];
			serial2.snd_tail %= SER_BUFF_SZ;
		}
		if(serial2.snd_head == serial2.snd_tail){
			USART2->CR1  &= ~(USART_CR1_TXEIE);
		}
	}
  /* USER CODE END USART2_IRQn 0 */
  //HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
  unsigned long isr = USART3->ISR;
	if(isr & USART_ISR_RXNE){

    serial3.rcv_buff[serial3.rcv_head++] = USART3->RDR;
    serial3.rcv_head %= SER_BUFF_SZ;
    serial3.rcv_tail += (serial3.rcv_head == serial3.rcv_tail);
    serial3.rcv_tail %= SER_BUFF_SZ;
	
	}
	else if(isr & USART_ISR_TXE){
    
		if(serial3.snd_head != serial3.snd_tail){
			USART3->TDR = serial3.snd_buff[serial3.snd_tail++];
			serial3.snd_tail %= SER_BUFF_SZ;
		}
		if(serial3.snd_head == serial3.snd_tail){
			USART3->CR1  &= ~(USART_CR1_TXEIE);
		}
	}
  /* USER CODE END USART3_IRQn 0 */
  //HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
  unsigned long isr = UART4->ISR;
	if(isr & USART_ISR_RXNE){
    
    serial4.rcv_buff[serial4.rcv_head++] = UART4->RDR;
    serial4.rcv_head %= SER_BUFF_SZ;
    serial4.rcv_tail += (serial4.rcv_head == serial4.rcv_tail);
    serial4.rcv_tail %= SER_BUFF_SZ;
	
	}
	else if(isr & USART_ISR_TXE){
    
		if(serial4.snd_head != serial4.snd_tail){
			UART4->TDR = serial4.snd_buff[serial4.snd_tail++];
			serial4.snd_tail %= SER_BUFF_SZ;
		}
		if(serial4.snd_head == serial4.snd_tail){
			UART4->CR1  &= ~(USART_CR1_TXEIE);
		}
	}
  /* USER CODE END UART4_IRQn 0 */
  //HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */
  unsigned long isr = UART5->ISR;
	if(isr & USART_ISR_RXNE){
    
    serial5.rcv_buff[serial5.rcv_head++] = UART5->RDR;
    serial5.rcv_head %= SER_BUFF_SZ;
    serial5.rcv_tail += (serial5.rcv_head == serial5.rcv_tail);
    serial5.rcv_tail %= SER_BUFF_SZ;
	
	}
	else if(isr & USART_ISR_TXE){
    
		if(serial5.snd_head != serial5.snd_tail){
			UART5->TDR = serial5.snd_buff[serial5.snd_tail++];
			serial5.snd_tail %= SER_BUFF_SZ;
		}
		if(serial5.snd_head == serial5.snd_tail){
			UART5->CR1  &= ~(USART_CR1_TXEIE);
		}
	}
  /* USER CODE END UART5_IRQn 0 */
  //HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */

  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles I2C4 event interrupt.
  */
void I2C4_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C4_EV_IRQn 0 */
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
