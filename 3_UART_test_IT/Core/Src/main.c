/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */


#include "stm32f4xx_hal.h"
#include<string.h>
#include<ctype.h>
#include<stdint.h>
#include "main.h"

#define CODE1 1

#define TRUE 			1
#define FALSE			0
uint8_t RECEPTION_CMPLT = FALSE;

void SystemClockConfig(void);
void UART2_Init(void);
void Err_Handler(void);

UART_HandleTypeDef HUart2;

char *user_data = "The app is running\r\n";

#ifdef CODE1

uint8_t pRxBuffer;
static uint16_t count = 0;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	GPIO_InitTypeDef led;

	led.Pin = GPIO_PIN_5;
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pull = GPIO_NOPULL;
	led.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOA, &led);


	HAL_UART_Transmit(&HUart2, (uint8_t *)user_data, strlen(user_data), HAL_MAX_DELAY);


	while(RECEPTION_CMPLT != TRUE)
		HAL_UART_Receive_IT(&HUart2, &pRxBuffer , 1);

	while(1)
	{
	}
}
void HAL_SYSTICK_Callback(void)
{

	count++;
	if(count == 1000)
	{
		count = 0;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}
void SystemClockConfig(void)
{

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


void Err_Handler(void)
{
	while(1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static int count = 0;
	static char TxBuffer[20];

	if(pRxBuffer == '\r')
	{
		RECEPTION_CMPLT = TRUE;
		TxBuffer[count++] = '\n';
		HAL_UART_Transmit(&HUart2, (uint8_t *)TxBuffer, strlen(TxBuffer), HAL_MAX_DELAY);

	}else
	{
		TxBuffer[count++] = pRxBuffer;

	}

}

#endif


#ifdef CODE2

uint8_t pRxBuffer[20];

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();
	GPIO_Init();

	HAL_UART_Transmit(&HUart2, (uint8_t *)user_data, strlen(user_data), HAL_MAX_DELAY);

	HAL_UART_Receive_IT(&HUart2, pRxBuffer , 6);

	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(1000);
	}
}

void SystemClockConfig(void)
{

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

	GPIO_InitTypeDef led;

	led.Pin = GPIO_PIN_5;
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pull = GPIO_NOPULL;
	led.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOA, &led);
}

void Err_Handler(void)
{
	while(1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static char TxBuffer[20];

	strcpy(TxBuffer,(char *)pRxBuffer);
	HAL_UART_Transmit(&HUart2, (uint8_t *)TxBuffer, strlen(TxBuffer), HAL_MAX_DELAY);

}


#endif
