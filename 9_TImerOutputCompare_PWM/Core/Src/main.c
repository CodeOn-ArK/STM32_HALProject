/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"

void SystemClockConfigHSE(uint8_t );
void UART2_Init(void);
void Err_Handler(void);
void CallUART(void);
void GPIO_Init(void);
void TIMER2_Init();

TIM_HandleTypeDef htimer2;


int main(void)
{
	HAL_Init();

	SystemClockConfigHSE(RCC_SYSCLK_FREQ_50MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well

	UART2_Init();
	GPIO_Init();
	TIMER2_Init();

	if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_2) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_3) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_4) != HAL_OK)
	{
		Err_Handler();
	}

	while(1);

}

/************************************************************************************************
 *
 * To work with timer's O/P compare channels in PWM mode we have to use 2 API's :
 *
 * 1) HAL_TIM_PWM_Init(<TIMER handle>); TO initialize the timers
 *
 * 2) HAL_TIM_PWM_ConfigChannel(<Handles>); To initialise the timer as PWM
 * 															and setup several features of PWM
 *
 ***********************************************************************************************/
void TIMER2_Init()
{
	TIM_OC_InitTypeDef TIMER_Handle;

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 49;
	htimer2.Init.Period = 1000 - 1;
	if(HAL_TIM_PWM_Init(&htimer2) != HAL_OK)
	{
		Err_Handler();
	}

	TIMER_Handle.OCMode 	= TIM_OCMODE_PWM1;
	TIMER_Handle.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIMER_Handle.Pulse 		= ((htimer2.Init.Period * 25.0) / 100.0);
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &TIMER_Handle, TIM_CHANNEL_1) != HAL_OK)
	{
		Err_Handler();
	}

	TIMER_Handle.OCPolarity = TIM_OCPOLARITY_LOW;
	TIMER_Handle.Pulse 		= ((htimer2.Init.Period * 25.0) / 100.0);
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &TIMER_Handle, TIM_CHANNEL_2) != HAL_OK)
	{
		Err_Handler();
	}

	TIMER_Handle.OCMode 	= TIM_OCMODE_PWM2;		//Negative of PWM1
	TIMER_Handle.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIMER_Handle.Pulse 		= ((htimer2.Init.Period * 25.0) / 100.0);
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &TIMER_Handle, TIM_CHANNEL_3) != HAL_OK)
	{
		Err_Handler();
	}

	TIMER_Handle.OCPolarity = TIM_OCPOLARITY_LOW;
	TIMER_Handle.Pulse 		= ((htimer2.Init.Period * 25.0) / 100.0);
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &TIMER_Handle, TIM_CHANNEL_4) != HAL_OK)
	{
		Err_Handler();
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
}

