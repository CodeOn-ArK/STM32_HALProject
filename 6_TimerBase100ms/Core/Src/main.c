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
	SystemClock_Config();

	UART2_Init();
	GPIO_Init();
	TIMER7_Init();

	CallUART();
	//Timer starts working from here
	HAL_TIM_Base_Start(&hTimer7);

	//Loop until the update event flag is set
	while(1){
		while( TIM7->SR & TIM_SR_UIF )
		{
			TIM7->SR = 0;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}
	}

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
	hTimer7.Init.Prescaler = 24;
	hTimer7.Init.Period = 64000-1;

	if(HAL_TIM_Base_Init(&hTimer7) != HAL_OK)
	{
		Err_Handler();
	}
}

