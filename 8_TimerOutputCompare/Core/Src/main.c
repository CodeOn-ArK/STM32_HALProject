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

#define  Pulse_500   25000	//Pulse value to Produce 500Hz
#define	 Pulse_1000  12500//Pulse value to Produce 1KHz
#define  Pulse_2000  6250	//Pulse value to Produce 2KHz
#define  Pulse_4000  3125		//Pulse value to Produce 4Khz
uint32_t CCR_content = 0;

int main(void)
{
	HAL_Init();

	SystemClockConfigHSE(RCC_SYSCLK_FREQ_50MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well

	UART2_Init();
	GPIO_Init();
	TIMER2_Init();

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK)
	{
		Err_Handler();
	}


	while(1);

}

/************************************************************************************************
 *
 * To work with timer's O/P compare channels we have to use 2 API's :
 *
 * 1)	HAL_TIM_OC_Init(<Timer handle>)	; Initializes the Timer's o/p Compare Time Base
 *
 * 2) 	HAL_TIM_OC_ConfigChannel()		; Configures the o/p Channel of the Timer using
 * 																		TIM_OC_InitTypeDef variable
 *
 ***********************************************************************************************/
void TIMER2_Init()
{

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK)
	{
		Err_Handler();
	}

	/*
	 * Calculations:
	 *
	 * TIMER clk : 25MHz = 25,000,000 Hz
	 * Desired Freq on Channel	=	500Hz
	 * Time Period = 0.002s
	 * Pulse Width = 0.001s
	 *
	 *		<-------->
	 *		____0.002s____      ____      ____      ____
	 * ____|    |____|    |____|    |____|    |____|    |
	 *		<--->
	 *		0.001s
	 *		(1000Hz)
	 *						      25,000,000
	 *	Therefore Pulse value:   ------------ = 25,000
	 *							    1000
	 */
	TIM_OC_InitTypeDef TIM2_OC_s;
	TIM2_OC_s.OCMode = TIM_OCMODE_TOGGLE;
	TIM2_OC_s.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM2_OC_s.Pulse =	Pulse_500;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &TIM2_OC_s, TIM_CHANNEL_1) != HAL_OK)
	{
		Err_Handler();
	}

	//TIM2_OC_s.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM2_OC_s.Pulse =	Pulse_1000;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &TIM2_OC_s, TIM_CHANNEL_2) != HAL_OK)
	{
		Err_Handler();
	}

	//TIM2_OC_s.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM2_OC_s.Pulse =	Pulse_2000;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &TIM2_OC_s, TIM_CHANNEL_3) != HAL_OK)
	{
		Err_Handler();
	}

	//TIM2_OC_s.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM2_OC_s.Pulse =	Pulse_4000;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &TIM2_OC_s, TIM_CHANNEL_4) != HAL_OK)
	{
		Err_Handler();
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	//TIM2_CH1 toggling with freq = 500Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,  HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + Pulse_500);
	}

	//TIM2_CH2 toggling with freq = 1000Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) + Pulse_1000);
	}

	//TIM2_CH3 toggling with freq = 2000Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3) + Pulse_2000);
	}

	//TIM2_CH4 toggling with freq = 4000Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4) + Pulse_4000);
	}


}

