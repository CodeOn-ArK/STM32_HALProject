/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "main.h"

void SystemClockConfig(uint8_t );
void UART2_Init(void);
void Err_Handler(void);
void CallUART(void);
void GPIO_Init(void);

UART_HandleTypeDef HUart2;
char msg[20];

int main(void)
{
	HAL_Init();

#if 0
	SystemClockConfig(RCC_SYSCLK_FREQ_25MHZ);
	GPIO_Init();
	CallUART();
#endif

#if 0
	SystemClockConfig(RCC_SYSCLK_FREQ_50MHZ);
	GPIO_Init();
	CallUART();
#endif

	SystemClockConfig(RCC_SYSCLK_FREQ_120MHZ);
	GPIO_Init();
	CallUART();

	SystemClockConfig(RCC_SYSCLK_FREQ_50MHZ); //This line gives error
	CallUART();

	while(1);
}

void SystemClockConfig(uint8_t SysFreq)
{
	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;

	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0 ,sizeof(ClkInit));

	uint8_t FLatency = 0;

	OscInit.OscillatorType 		= RCC_OSCILLATORTYPE_HSI;
	OscInit.HSIState 			= RCC_HSI_ON;
	OscInit.HSICalibrationValue = 0x10;
	OscInit.PLL.PLLState 		= RCC_PLL_ON;
	OscInit.PLL.PLLSource 		= RCC_PLLSOURCE_HSI;

	switch(SysFreq)
	{
	case RCC_SYSCLK_FREQ_25MHZ:
		OscInit.PLL.PLLM		= 8;
		OscInit.PLL.PLLN		= 100;
		OscInit.PLL.PLLP 		= 8;

		ClkInit.ClockType 	  	= (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource  	= RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider 	= RCC_HCLK_DIV2;
		ClkInit.APB2CLKDivider 	= RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_0WS;

		break;

	case RCC_SYSCLK_FREQ_50MHZ:
		OscInit.PLL.PLLM 		 = 8;
		OscInit.PLL.PLLN 		 = 100;
		OscInit.PLL.PLLP 		 = 4;

		ClkInit.ClockType 	 	 = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource 	 = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider	 = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider	 = RCC_HCLK_DIV2;
		ClkInit.APB2CLKDivider	 = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_1WS;

		break;

	case RCC_SYSCLK_FREQ_120MHZ:
		OscInit.PLL.PLLM 		 = 8;
		OscInit.PLL.PLLN 		 = 120;
		OscInit.PLL.PLLP 		 = 2;

		ClkInit.ClockType 	  	 = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource  	 = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider 	 = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider 	 = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider 	 = RCC_HCLK_DIV4;

		FLatency = FLASH_ACR_LATENCY_3WS;

		break;

	}

	if(HAL_RCC_OscConfig(&OscInit) != HAL_OK)
	{
		Err_Handler();
	}

	if(HAL_RCC_ClockConfig(&ClkInit, FLatency) != HAL_OK)
	{
		Err_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);


}

void UART2_Init(void)
{
	HUart2.Instance = USART2;
	HUart2.Init.BaudRate = 115200;
	HUart2.Init.WordLength = UART_WORDLENGTH_8B;
	HUart2.Init.StopBits = UART_STOPBITS_1;
	HUart2.Init.Parity = UART_PARITY_NONE;
	HUart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HUart2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&HUart2) != HAL_OK)
	{
		//Problem occured
		Err_Handler();
	}
}

void GPIO_Init()
{
	GPIO_InitTypeDef Gpio;

	Gpio.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio.Pin = GPIO_PIN_5;
	Gpio.Pull = GPIO_NOPULL;
	Gpio.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOA, &Gpio);

}

void CallUART()
{
	UART2_Init();

	sprintf(msg, "SYSCLK : %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&HUart2,(uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	sprintf(msg, "HCLK   : %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&HUart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	sprintf(msg, "PCLK1  : %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&HUart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	sprintf(msg, "PCLK2  : %ld\r\n\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&HUart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

}

void Err_Handler(void)
{
	GPIO_Init();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

	while(1);
}

