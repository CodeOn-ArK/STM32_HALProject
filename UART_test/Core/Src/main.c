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

void SystemClockConfig(void);
void UART2_Init(void);
void Err_Handler(void);

UART_HandleTypeDef HUart2;

//char *user_data = "The app is running\r\n";

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint8_t pRxBuffer[30];
	uint8_t len;


	for(uint8_t i = 0; ;i++)
	{
		if(HAL_UART_Receive(&HUart2, &pRxBuffer[i], 1, HAL_MAX_DELAY) != HAL_OK)
			Err_Handler();
		if(pRxBuffer[i] == '\r')
			break;
	}

	for(uint8_t i=0; ; i++ )
	{
		if(pRxBuffer[i] == '\r')
			break;

		if(pRxBuffer[i] >= 97 && pRxBuffer[i] <= 122)
			pRxBuffer[i] -= 32;
	}

	//strcat(pRxBuffer, '\r');
	if(HAL_UART_Transmit(&HUart2, (uint8_t *)pRxBuffer, strlen((char *)pRxBuffer), HAL_MAX_DELAY) != HAL_OK) Err_Handler();

	(void)pRxBuffer;
	(void)len;
	while(1);
}UART_test2UART_test2

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
