/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"
#include<stdarg.h>

void printMsg(char *format,...);

void printMsg(char *format,...)
{
	char str[80];

	/*extract the arg list using VA APIs */
	va_list args;
	va_start(args, format);
	vsprintf(str, format, args);

	HAL_UART_Transmit(&HUart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	va_end(args);
}

int main(void)
{
	HAL_Init();

	SystemClockConfigHSI(RCC_SYSCLK_FREQ_25MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well

	while(1);

}

