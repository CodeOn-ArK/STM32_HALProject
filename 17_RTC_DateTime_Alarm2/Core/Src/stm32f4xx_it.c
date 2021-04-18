/*
 * it.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

#include "main.h"

extern RTC_HandleTypeDef HRtc;

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}

void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}

void RTC_Alarm_IRQHandler()
{
	HAL_RTC_AlarmIRQHandler(&HRtc);
}

