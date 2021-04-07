/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"

void UART2_Init(void);
void TIMER6_Init(void);
void CallUART(void);

TIM_HandleTypeDef TIMHandle;
extern UART_HandleTypeDef HUart2;
char pData[10] = "pressed\n\r";

int main(void)
{
	HAL_Init();

	SystemClock_Config();
	//SystemClockConfigHSE(RCC_SYSCLK_FREQ_50MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well
	TIMER6_Init();
	UART2_Init();
	GPIO_Init();

	TIM6->SR = 0x0;

	if(HAL_TIM_Base_Start_IT(&TIMHandle) != HAL_OK)
	{
		Err_Handler();
	}

	//SCB->SCR |= (1 << 1); //Or use HAL_PWR_EnableSleepOnExit()

	while(1);

}

void TIMER6_Init(void)
{

	TIMHandle.Instance = TIM6;
	TIMHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIMHandle.Init.Prescaler = 15;
	TIMHandle.Init.Period = 10000 - 1;
	if(HAL_TIM_Base_Init(&TIMHandle) != HAL_OK)
	{
		Err_Handler();
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(HAL_UART_Transmit(&HUart2, (uint8_t *)pData, strlen(pData), HAL_MAX_DELAY) != HAL_OK)
	{
		Err_Handler();
	}

}
