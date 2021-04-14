/*
 * it.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

#include "main.h"


void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}

void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

