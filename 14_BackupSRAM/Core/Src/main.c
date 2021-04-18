/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

/*
 * Prog to measure the current consumption in STANDBY mode
 *
 * - BKUP SRAM on && RTC off
 * - Observe the contents of BKUP SRAM before && after STANDBY mode
 */

#include "main.h"

int main(void)
{
	HAL_Init();

	SystemClockConfigHSI(RCC_SYSCLK_FREQ_25MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well

	while(1);

}

