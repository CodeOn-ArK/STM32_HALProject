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
#include<string.h>
#include<stdarg.h>

void printMsg(char *format,...);

uint8_t *pBackupSRAMBase ;
extern UART_HandleTypeDef HUart2;

char WriteBuf[] = "How are you in this world???? 12345!@#$%";

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

	GPIO_Init();
	UART2_Init();

	//1.Turn on the clk in RCC register for bkp SRAM
	__HAL_RCC_BKPSRAM_CLK_ENABLE();

	//2. Enable Write access to the bkp SRAM
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	pBackupSRAMBase = (uint32_t *)(BKPSRAM_BASE);

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		printMsg("\n\rWoke up from STANDBY Mode\n\r");

		//HAL_UART_Transmit(&HUart2, (uint8_t *)(pBackupSRAMBase), sizeof(WriteBuf), HAL_MAX_DELAY);
		uint8_t data;
		data = (uint8_t )*pBackupSRAMBase;
		if(data == 'H') HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}else{


		//strcpy((uint32_t *)pBackupSRAMBase, WriteBuf);
		for(uint16_t i=0; i<strlen(WriteBuf)+1; i++)
			*(pBackupSRAMBase + i) = WriteBuf[i];

	}

	printMsg("\n\rPress the switch to enter STANDBY mode\n\r");

	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != 0);

	for(uint32_t i=0; i<400000; i++);

	printMsg("\n\rEntering STANDBY Mode \n\r");

	HAL_PWREx_EnableBkUpReg();
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnterSTANDBYMode();


	while(1);

}

