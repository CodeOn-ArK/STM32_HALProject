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
 * 2)
 *************************************************************************************/

#include "main.h"

extern UART_HandleTypeDef HUart2;

void GPIO_AnalogConfig();

int main(void)
{
	HAL_Init();

	//SystemClock_Config();	//Choose HSI over PLL or HSE; consumes lesser power
	SystemClockConfigHSE(RCC_SYSCLK_FREQ_25MHZ);
	//__HAL_RCC_HSI_DISABLE(); use wisely live well
	GPIO_AnalogConfig();
	GPIO_Init();
	UART2_Init();

	while(1)
	{
		__WFI(); //Wait for Interrupt
				// This Instruction waits for an interrupt to wake up the processor; executing this makes the processor going to sleep immediately
	}

}


void GPIO_Init()
{
	/*
	 * Use only required Peripherals!!
	 * GPIOA not needed , but kept only for demonstration puraposes
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

	Gpio.Pin = GPIO_PIN_12;
	Gpio.Mode = GPIO_MODE_IT_RISING;
	Gpio.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOC, &Gpio);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	static int i = 0;
	char msg[50];
	if(GPIO_Pin == GPIO_PIN_12)
	{
		sprintf(msg,"Testing WFI instruction %d\n\r", i++);
		HAL_UART_Transmit_IT(&HUart2, (uint8_t*)msg, strlen(msg));
	}
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
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
