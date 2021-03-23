/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"

int main(void)
{
	HAL_Init();

	SystemClockConfigHSI(RCC_SYSCLK_FREQ_25MHZ);
	__HAL_RCC_HSI_DISABLE();

	while(1);

}

