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
char str[50];

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

	SystemClockConfigHSE(RCC_SYSCLK_FREQ_25MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well
	GPIO_Init();
	UART2_Init();

	RTC_Init();
	RTC_CalConfig();


	while(1)
	{
		//HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI );
	}

}

void RTC_Init()
{

	HRtc.Instance = RTC;
	HRtc.Init.HourFormat = RTC_HOURFORMAT_12;
	HRtc.Init.AsynchPrediv = 0x7F;
	HRtc.Init.SynchPrediv = 0xFF;
	HRtc.Init.OutPut = RTC_OUTPUT_DISABLE;

	if(HAL_RTC_Init(&HRtc) != HAL_OK) Err_Handler();

}

void RTC_CalConfig(void)
{
	//This function initilizes the RTC calender unit
//	memset(&HRtcDate,0, sizeof(HRtcDate));
//	memset(&HRtcTime,0, sizeof(HRtcTime));
	RTC_TimeTypeDef  HRtcTime;
	RTC_DateTypeDef  HRtcDate;

	HRtcTime.Hours 		= 0x11;
	HRtcTime.Minutes 	= 0x59;
	HRtcTime.Seconds 	= 0x30;
	HRtcTime.TimeFormat = RTC_HOURFORMAT12_PM;

	if(HAL_RTC_SetTime(&HRtc, &HRtcTime, RTC_FORMAT_BCD) != HAL_OK) Err_Handler();

	HRtcDate.Date 		= 16;
	HRtcDate.WeekDay 	= RTC_WEEKDAY_FRIDAY;
	HRtcDate.Month 		= RTC_MONTH_APRIL;
	HRtcDate.Year 		= 21;

	if(HAL_RTC_SetDate(&HRtc, &HRtcDate,RTC_FORMAT_BIN) != HAL_OK) Err_Handler();

}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{

	if(pin == GPIO_PIN_13)
	{
		RTC_TimeTypeDef  HRtcTimeR;
		RTC_DateTypeDef  HRtcDateR;

		if(HAL_RTC_GetTime(&HRtc, &HRtcTimeR, RTC_FORMAT_BIN) != HAL_OK) Err_Handler();
		if(HAL_RTC_GetDate(&HRtc, &HRtcDateR, RTC_FORMAT_BIN) != HAL_OK) Err_Handler();

		sprintf(str, "The time is %02d:%02d:%02d %s\n\r", HRtcTimeR.Hours, HRtcTimeR.Minutes,HRtcTimeR.Seconds, ((HRtcTimeR.TimeFormat == 0x0) ? " AM" : " PM"));
		printMsg(str);

		sprintf(str, "The date is %02d-%02d-%02d\n\n\r", HRtcDateR.Date , HRtcDateR.Month , HRtcDateR.Year);
		printMsg(str);

	}
}






