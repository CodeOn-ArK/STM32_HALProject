/*
 * it.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

#include "main.h"

extern TIM_HandleTypeDef TIMHandle;

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIMHandle);
}
