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
TIM_OC_InitTypeDef TIMER_Handle;

static volatile uint16_t brightness = 0;

int main(void)
{
	HAL_Init();

	SystemClockConfigHSE(RCC_SYSCLK_FREQ_50MHZ);
	//__HAL_RCC_HSI_DISABLE(); //use wisely live well

	UART2_Init();
	GPIO_Init();
	TIMER2_Init();

	if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Err_Handler();
	}

	static int flag = 0;

	while(1)
	{
		if(!flag)
		{
			brightness++;
			__HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);
			if(brightness == htimer2.Init.Period) flag = 1;
		}

		if(flag)
		{
			brightness--;
			__HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);
			if(brightness == 0x0) flag = 0;
		}
	};

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
	memset(&htimer2, 0, sizeof(htimer2));
	memset(&TIMER_Handle, 0, sizeof(TIMER_Handle));

	htimer2.Instance 	     = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler 	 = 49;
	htimer2.Init.Period 	 = 100 - 1;
	if(HAL_TIM_PWM_Init(&htimer2) != HAL_OK)
	{
		Err_Handler();
	}

	TIMER_Handle.OCMode 	= TIM_OCMODE_PWM1;
	TIMER_Handle.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIMER_Handle.Pulse 		= 0;
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &TIMER_Handle, TIM_CHANNEL_1) != HAL_OK)
	{
		Err_Handler();
	}

}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
}

