/*
 * it.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */
#include"stm32f4xx_hal.h"

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}
