/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"

void TIMER7_Init();
void SystemClock_Config(void);

TIM_HandleTypeDef hTimer7;

int main(void)
{
	HAL_Init();
	//SystemClock_Config();
	//SystemClockConfigHSE(RCC_SYSCLK_FREQ_25MHZ);

	SystemClockConfigHSI(RCC_SYSCLK_FREQ_25MHZ);

	UART2_Init();
	GPIO_Init();
	TIMER7_Init();

	CallUART();
	//Timer starts working from here in interrupt mode
	HAL_TIM_Base_Start_IT(&hTimer7);

	while(1);

}


void TIMER7_Init()
{
	/*
	 *  					  (TIM_CLK)
	 *  Update_Event =	  ~~~~~~~~~~~~~~~~
	 * 					((PSC + 1)(ARR + 1))
	 */
	hTimer7.Instance = TIM7;
	hTimer7.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTimer7.Init.Prescaler = 50;
	hTimer7.Init.Period = 50000-1;

	if(HAL_TIM_Base_Init(&hTimer7) != HAL_OK)
	{
		Err_Handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}


