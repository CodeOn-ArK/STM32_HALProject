/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

/************************************************************************************
 *
 * This project is aimed at reducing power consumption by limiting peripherals but not thier usages
 *
 * 1) Increasing UART speed to highest permissible and practical rate helps keeping current consumption low
 * 2) Using WFE instruction to put the processor in sleep and wait for an event to occur
 *
 *************************************************************************************/

#include "main.h"

extern UART_HandleTypeDef HUart2;

void SystemClock_Config();
void GPIO_AnalogConfig();

char msg[50];
static int i = 0;

int main(void)
{
	HAL_Init();

	SystemClock_Config();	//Choose HSI over PLL or HSE; consumes lesser power
	//SystemClockConfigHSE(RCC_SYSCLK_FREQ_25MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well
//	GPIO_AnalogConfig();
	GPIO_Init();
	UART2_Init();

	while(1)
	{

		sprintf(msg,"Testing WFE instruction %d\n\r", i++);

		if(HAL_UART_Transmit(&HUart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK)
		{
			Err_Handler();
		}

		/*
		 * Processor needs to go to sleep but systick prevents it from going to sleep as it generates an event every 1ms
		 * Hence to prevent systick from generating unnecessary events during sleep we suspend systick here
		 */
		HAL_SuspendTick();

		__WFE();	//Wait for Event
					// This Instruction waits for an interrupt to wake up the processor; executing this makes the processor going to sleep immediately
					//if an event is not pending; if any is pending first it is cleared and then processor continues normal execution without going to sleep
		__WFE();	// 2 WFI helps to slow down input process

		HAL_ResumeTick();		//Resumes systick
	}

}


void GPIO_Init()
{
	/*
	 * Use only required Peripherals!!
	 * GPIOA not needed , but kept only for demonstration purposes
	 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE(); // Can't be done for PORTC as that is our WAKEUP source,
										// which needs to be alive when processor goes to sleep


	GPIO_InitTypeDef Gpio;

	Gpio.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio.Pin = GPIO_PIN_5;
	Gpio.Pull = GPIO_NOPULL;
	Gpio.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOA, &Gpio);

	Gpio.Pin = GPIO_PIN_13;
	Gpio.Mode = GPIO_MODE_EVT_FALLING;
	Gpio.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOC, &Gpio);

	//HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
	//HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	//HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//no need
}

void GPIO_AnalogConfig(void)
{
	/*
	 * Making UNUSED GPIOs as ANALOG reduces power consumption
	 * Here we have changed every pin from default INPUT mode to ANALG mode
	 */
	GPIO_InitTypeDef GPIOClear;

	GPIOClear.Pin =  ~(GPIO_PIN_5 | GPIO_PIN_2 | GPIO_PIN_3); //PA5, PA2, PA3 to be left out
	GPIOClear.Mode = GPIO_MODE_ANALOG;

	HAL_GPIO_Init(GPIOA, &GPIOClear);

	GPIOClear.Pin =  ~(GPIO_PIN_12); //PA12 to be left out
	GPIOClear.Mode = GPIO_MODE_ANALOG;

	HAL_GPIO_Init(GPIOC, &GPIOClear);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
}
