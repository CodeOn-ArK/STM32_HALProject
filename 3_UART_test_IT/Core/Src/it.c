/*
 * it.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

#include "main.h"

extern UART_HandleTypeDef HUart2;

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}


void USART2_IRQHandler(void)
{

	HAL_UART_IRQHandler(&HUart2);
}
