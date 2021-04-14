/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"
#include<stdarg.h>

void printMsg(char *format,...);
void RTC_Init(void);
void RTC_CalConfig(void);

extern UART_HandleTypeDef HUart2;
RTC_HandleTypeDef HRtc;
RTC_TimeTypeDef  HRtcTime;
RTC_DateTypeDef  HRtcDate;

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

	RTC_Init();

	while(1);

}

void RTC_Init()
{

	HRtc.Instance = RTC;
	HRtc.Init.HourFormat = RTC_HOURFORMAT_24;
	HRtc.Init.AsynchPrediv = 127;
	HRtc.Init.SynchPrediv = 255;
	HRtc.Init.OutPut = RTC_OUTPUT_DISABLE;

	if(HAL_RTC_Init(&HRtc) != HAL_OK) Err_Handler();

}

void RTC_CalConfig(void)
{
	//This function initilizes the RTC calender unit

	HRtcTime.Hours = 0x19;
	HRtcTime.Minutes = 0x01;
	HRtcTime.Seconds = 0x00;

	if(HAL_RTC_SetTime(&HRtc, &HRtcTime, RTC_FORMAT_BCD) != HAL_OK) Err_Handler();

	HRtcDate.Date = 0x14;
	HRtcDate.WeekDay = RTC_WEEKDAY_TUESDAY;
	HRtcDate.Month = RTC_MONTH_APRIL;
	HRtcDate.Year = 0x21;

	if(HAL_RTC_SetDate(&HRtc, &HRtcDate,RTC_FORMAT_BCD) != HAL_OK) Err_Handler();

}

void HAL_GPIO_EXTI_IRQHandler(uint16_t pin)
{
	char str[40];

	if(pin == GPIO_PIN_13)
	{
		if(HAL_RTC_GetTime(&HRtc, &HRtcTime, RTC_FORMAT_BCD) != HAL_OK) Err_Handler();
	}
}























