/*
 * msp.c
 *
 *  Created on: Feb 28, 2021
 *      Author: ark
 */

#include "stm32f4xx_hal.h"

#define USAGEFAULTEN 	18
#define BUSFAULTEN 		17
#define MEMFAULTEN 		16

void HAL_MspInit(void)
{
	//Here low low level processor specific initialization is done
	//1. The priority grouping of the arm Cortex Mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);			//This is default in hal_cortex.c ;
																//making this line of code redundant


	//2. Enable the required system exceptions od the ARM Cortex Mx processor
	SCB->SHCSR |= ((0x1 << BUSFAULTEN) | (0x1 << USAGEFAULTEN) | (0x1 << MEMFAULTEN));

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//This funx does the low level initializiation of the peripheral

	//1.Enable the clock for the USART2 peripheral
	__HAL_RCC_GPIOA_CLK_ENABLE();  //Macro to enable GPIO clk
	__HAL_RCC_USART2_CLK_ENABLE(); //Macro to enable USART2 clk

	//2.Do the pin muxing
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;  //USART2 Tx
	HAL_GPIO_Init(GPIOA, &gpio_uart); 		//Tx configuration

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart); 		//Rx configuration

	//3.Enable the IRQ and set up the priority (NVIC Setting)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}
/*******************************************************************************************************
 * 			THIS SECTION IS FOR USE WITH MAIN.C FILE TO DECLUTTER THE MAIN SECTION FROM UNWANTED
 * 											REFERENCES
 *
 *******************************************************************************************************/

#include"main.h"  //This is to include the references used in main.h

UART_HandleTypeDef HUart2;
char msg[50];

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Err_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Err_Handler();
  }
}

void SystemClockConfigHSE(uint8_t clk_freq)
{

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;

	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0 ,sizeof(ClkInit));

	uint8_t FLatency = 0;

	OscInit.OscillatorType 		= RCC_OSCILLATORTYPE_HSE;
	OscInit.HSEState 			= RCC_HSE_BYPASS;
	OscInit.PLL.PLLState 		= RCC_PLL_ON;
	OscInit.PLL.PLLSource 		= RCC_PLLSOURCE_HSE;

	switch(clk_freq)
	{
	case RCC_SYSCLK_FREQ_25MHZ:
		OscInit.PLL.PLLM		= 8;
		OscInit.PLL.PLLN		= 100;
		OscInit.PLL.PLLP 		= 4;

		ClkInit.ClockType 	  	= (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource  	= RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider 	= RCC_HCLK_DIV1;
		ClkInit.APB2CLKDivider 	= RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_0WS;

		break;

	case RCC_SYSCLK_FREQ_50MHZ:
		OscInit.PLL.PLLM 		 = 8;
		OscInit.PLL.PLLN 		 = 100;
		OscInit.PLL.PLLP 		 = 2;

		ClkInit.ClockType 	 	 = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource 	 = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider	 = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider	 = RCC_HCLK_DIV2;
		ClkInit.APB2CLKDivider	 = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_1WS;

		break;

	case RCC_SYSCLK_FREQ_120MHZ:
		OscInit.PLL.PLLM 		 = 4;
		OscInit.PLL.PLLN 		 = 120;
		OscInit.PLL.PLLP 		 = 2;

		ClkInit.ClockType 	  	 = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource  	 = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider 	 = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider 	 = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider 	 = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_3WS;

		break;

	case RCC_SYSCLK_FREQ_180MHZ:

		//Enable the clock for power controller
		__HAL_RCC_PWR_CLK_ENABLE();

		//Set regulator voltage scale as 1
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		//Turn on the over drive mode of the voltage regulator
		__HAL_PWR_OVERDRIVE_ENABLE();

		OscInit.PLL.PLLM 		 = 4;
		OscInit.PLL.PLLN 		 = 180;
		OscInit.PLL.PLLP 		 = 2;

		ClkInit.ClockType 	  	 = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource  	 = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider 	 = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider 	 = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider 	 = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_5WS;

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

void SystemClockConfigHSI(uint8_t SysFreq)
{

	if(SysFreq == RCC_SYSCLK_FREQ_180MHZ)
	{
		sprintf(msg, "180 MHz not supported for HSI");
		HAL_UART_Transmit(&HUart2,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

		sprintf(msg, "\nDefaulting to 50 MHz");
		SystemClockConfigHSI(RCC_SYSCLK_FREQ_50MHZ);
	}

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

/*************************************************************************************************/

