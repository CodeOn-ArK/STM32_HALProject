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
#include<stdio.h>
#include "main.h"

void SystemClockConfig(void);
void UART2_Init(void);
void Err_Handler(void);
void CallUART(void);

#define TRUE 	1
#define FALSE 	0

UART_HandleTypeDef HUart2;
char msg[20];

int main(void)
{
	HAL_Init();

	CallUART();

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;

	memset(&msg, 0 ,sizeof(msg));
	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0, sizeof(ClkInit));
	/*
	 * THe Osc is being configured here to source HSE
	 * from F103 uC (8MHz)
	 */

	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscInit.HSEState = RCC_HSE_BYPASS;

	if(HAL_RCC_OscConfig(&OscInit) != HAL_OK)
	{
		Err_Handler();
	}

	/*
	 * The system will switch clock from HSI to HSE
	 * after execution of the following routines
	 *
	 * Also sets up prescaler for AHB, APB1 && APB2
	 */

	ClkInit.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK \
								| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_HSE;
	ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV2;
	ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;
	ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&ClkInit, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Err_Handler();
	}

	__HAL_RCC_HSI_DISABLE(); //Saves power

	/*------------------------AFTER THIS LINE SYSCLK IS SOURCED BY HSE------------------------*/
	//SYSTICK Reconfiguration
	/*
	 * In this part we have to reconfigure the tick value which is to be feeded into the
	 * SYSTICK API.
	 * The SYSTICK derives its freq from HCLK
	 * Earlier the Freq was being sourced from HSI(16MHz).
	 * Now the HCLK sources freq from HSE(8MHz) with a prescalar of 2; FHCLK  =  4MHz
	 * Hence we need to change after how many ticks(number of ticks) the SYSTICK will interrupt
	 * the SYSTICK interrupts after every 1ms so we need to calc accordingly
	 *
	 * HCLK = 4MHz
	 *
	 * =>0.25 uS -----> 1tick
	 * =>1 ms    -----> ?ticks
	 *
	 * 4000 ticks are required
	 */

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/*----------------------------------------------------------------------------------------*/

	CallUART();

	while(1);
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
	while(1);
}

