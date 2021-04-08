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

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&HUart2);
}
