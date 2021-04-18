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
char* getDate(uint8_t day);
void RTC_AlarmConfig();

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
	//RTC_CalConfig();


	while(1);

}

void RTC_Init()
{
	/*
	 * Init the RTC in 24 Hr frmt(_RTC alarm is only activated in 24Hr format, 12 Hr is don't care here_)
	 */
	memset(&HRtc, 0, sizeof(HRtc));

	HRtc.Instance = RTC;
	HRtc.Init.HourFormat = RTC_HOURFORMAT_24;
	HRtc.Init.AsynchPrediv = 0x7F;
	HRtc.Init.SynchPrediv = 0xFF;
	HRtc.Init.OutPut = RTC_OUTPUT_DISABLE;

	if(HAL_RTC_Init(&HRtc) != HAL_OK) Err_Handler();

}

void RTC_CalConfig(void)
{
	//This function initilizes the RTC calender unit
	RTC_TimeTypeDef  HRtcTime;
	RTC_DateTypeDef  HRtcDate;

	memset(&HRtcDate,0, sizeof(HRtcDate));
	memset(&HRtcTime,0, sizeof(HRtcTime));

	//The current date is entered here
	HRtcTime.Hours 		= 0x11;
	HRtcTime.Minutes 	= 0x59;
	HRtcTime.Seconds 	= 0x00;

	//This API is called to set the time in BCD format( Use Hex notation for BCD frmt )
	if(HAL_RTC_SetTime(&HRtc, &HRtcTime, RTC_FORMAT_BCD) != HAL_OK) Err_Handler();

	//The current date is entered here
	HRtcDate.Date 		= 18;
	HRtcDate.WeekDay 	= RTC_WEEKDAY_SUNDAY;
	HRtcDate.Month 		= RTC_MONTH_APRIL;
	HRtcDate.Year 		= 21;

	//This API is called to set the date in BIN frmt
	if(HAL_RTC_SetDate(&HRtc, &HRtcDate,RTC_FORMAT_BIN) != HAL_OK) Err_Handler();

	/*
	 * This setup is lost everytime the board is reset since the LPR is not configured yet;
	 * hence a board reset, resets the RTC to the time entered here
	 */
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{

	if(pin == GPIO_PIN_12)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);

		RTC_TimeTypeDef  HRtcTimeR;
		RTC_DateTypeDef  HRtcDateR;

		memset(&HRtcTimeR, 0, sizeof(HRtcTimeR));
		memset(&HRtcDateR, 0, sizeof(HRtcDateR));

		if(HAL_RTC_GetTime(&HRtc, &HRtcTimeR, RTC_FORMAT_BIN) != HAL_OK) Err_Handler();
		if(HAL_RTC_GetDate(&HRtc, &HRtcDateR, RTC_FORMAT_BIN) != HAL_OK) Err_Handler();

		sprintf(str, "The time is %02d:%02d:%02d %s\n\r", HRtcTimeR.Hours, HRtcTimeR.Minutes,HRtcTimeR.Seconds, ((HRtcTimeR.TimeFormat == RTC_HOURFORMAT12_AM) ? " AM" : " PM"));
		printMsg(str);

		printMsg("The date is %02d-%02d-%02d  (%s)\n\n\r", HRtcDateR.Date , HRtcDateR.Month , HRtcDateR.Year, getDate(HRtcDateR.WeekDay));

		RTC_AlarmConfig();

	}
}

char* getDate(uint8_t day)
{
	char *weekDay[] = {"Monday", "Tuesday", "Wednesday","Thursday","Friday","Saturday","Sunday"};

	return weekDay[day - 1];
}


void RTC_AlarmConfig()
{
	//Set an alarm for 12:00:09 everyday
	RTC_AlarmTypeDef RTC_AlarmA_h;

	memset(&RTC_AlarmA_h, 0, sizeof(RTC_AlarmA_h));

	RTC_AlarmA_h.Alarm = RTC_ALARM_A;
	RTC_AlarmA_h.AlarmTime.Hours = 0x12;
	RTC_AlarmA_h.AlarmTime.Minutes = 0x00;
	RTC_AlarmA_h.AlarmTime.Seconds = 0x09;
//	RTC_AlarmA_h.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;

	RTC_AlarmA_h.AlarmMask =  RTC_ALARMMASK_DATEWEEKDAY ;
	RTC_AlarmA_h.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	//Init the alarm
	if(HAL_RTC_SetAlarm_IT(&HRtc, &RTC_AlarmA_h, RTC_FORMAT_BCD) != HAL_OK)
	{
		Err_Handler();
	}

	printMsg("Alarm is set\n\r");
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	//This callback is a part of the ISR so keep it short
	printMsg("\n\r Alarm Triggered \n\r");


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

}


