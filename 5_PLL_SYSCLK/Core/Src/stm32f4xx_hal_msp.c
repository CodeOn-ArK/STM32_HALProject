/*
 * msp.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

#include "stm32f4xx_hal.h"

#define USAGEFAULTEN 	18
#define BUSFAULTEN 		17
#define MEMFAULTEN 		16

void HAL_MspInit(void)
{
	//Here low low level processor specific initialization is done
	//1. The priority grouping of the arm Cortex Mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);			//This is default in hal_cortex.c ;
																//making this line of code redundant


	//2. Enable the required system exceptions od the ARM Cortex Mx processor
	SCB->SHCSR |= ((0x1 << BUSFAULTEN) | (0x1 << USAGEFAULTEN) | (0x1 << MEMFAULTEN));

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//This funx does the low level initializiation of the peripheral

	//1.Enable the clock for the USART2 peripheral
	__HAL_RCC_GPIOA_CLK_ENABLE();  //Macro to enable GPIO clk
	__HAL_RCC_USART2_CLK_ENABLE(); //Macro to enable USART2 clk

	//2.Do the pin muxing
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;  //USART2 Tx
	HAL_GPIO_Init(GPIOA, &gpio_uart); 		//Tx configuration

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart); 		//Rx configuration

	//3.Enable the IRQ and set up the priority (NVIC Setting)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
}
