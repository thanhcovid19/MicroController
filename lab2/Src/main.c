/* USER CODE BEGIN Header */
/* this file belong to stm32_second_project*/
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author	        : Nguyen Thanh Vinh
  * @date		    : 21/10/2021
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	LED1_ON,
	LED2_ON,
	LED3_ON,
	LED4_ON
}LED_STATE;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define DELAY_TIME 			1000
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
int counter = 50;     // if the frequency of 4 seven leds is 1Hz => set counter = 24
//enum STATE state = first_led_on;
int column_shift = 0;
int column_scan = 0;
int timer_blink_2leds = 4;
const int MAX_LED = 4;
const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;
int index_led = 0;
int led_buffer[4] = {6,7,8,9};
unsigned int matrix_led_buffer[8] = {0xffff, 0x83ff, 0xedff, 0xeeff, 0xeeff, 0xedff, 0x83ff, 0xffff};
unsigned int colums[8] = {0xfeff, 0xfdff, 0xfbff, 0xf700, 0xef00, 0xdf00, 0xbf00, 0x7f00};
int hour = 0, minute = 0, second = 0;
int timer0_counter = 0;
int timer0_flag = 0;
int TIMER_CYCLE = 10;
int switch_led_flag = 0;
int switch_led_time = 25;
LED_STATE led_state = LED1_ON;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void displayMatrixLed(int num);
void upadateLedMatrix(int index);
void display7SEG(int num);
void delay_half_of_sec();
void update7SEG(int index);
void updateClockBuffer();
void setTimer0(int duration);
void timer_run();
void show_clock();
void calculate_time();
void enable_colums();
void shift_left_to_right();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void shift_left_to_right(){
	unsigned int temp = matrix_led_buffer[7];
	for(int i = 7; i > 0; i--){
		matrix_led_buffer[i] = matrix_led_buffer[i-1];
	}

	matrix_led_buffer[0] = temp;
}

void displayMatrixLed(int num){
	GPIOB->ODR = matrix_led_buffer[num];
	GPIOA->ODR = colums[num];
}

void updateLedMatrix(int index){
	switch(index){
		case 0:
		{
			displayMatrixLed(0);
			break;
		}
		case 1:
		{
			displayMatrixLed(1);
			break;
		}
		case 2:
		{
			displayMatrixLed(2);
			break;
		}
		case 3:{
			displayMatrixLed(3);
			break;
		}
		case 4:
		{
			displayMatrixLed(4);
			break;
		}
		case 5:
		{
			displayMatrixLed(5);
			break;
		}
		case 6:
		{
			displayMatrixLed(6);
			break;
		}
		case 7:
		{
			displayMatrixLed(7);
			break;
		}
	}
}

void show_clock(){
	switch(led_state){
		case LED1_ON:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			//display7SEG(led_buffer[0]);
			update7SEG(0);
			led_state = LED2_ON;
			break;
		}
		case LED2_ON:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			//display7SEG(led_buffer[1]);
			update7SEG(1);
			led_state = LED3_ON;
			break;
		}
		case LED3_ON:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, RESET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			//display7SEG(led_buffer[2]);
			update7SEG(2);
			led_state = LED4_ON;
			break;
		}
		case LED4_ON:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, RESET);
			//display7SEG(led_buffer[3]);
			update7SEG(3);
			led_state = LED1_ON;
			break;
		}
	}
//	if(timer_blink_2leds <= 1){
//		timer_blink_2leds = 4;
//		HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
//	}
//	timer_blink_2leds--;
}

void calculate_time(){
	second++;
	if(second > 10){
	  second = 0;
	  minute++;
	}

	if(minute > 10){
	  minute = 0;
	  hour++;
	}

	if(hour > 5){
	  hour = 0;
	}
	updateClockBuffer();
}

void setTimer0(int duration){
	timer0_counter = duration/TIMER_CYCLE;
	timer0_flag = 0;
}

void timer_run(){
	if(timer0_counter > 0){
		timer0_counter = timer0_counter - 1;
		if(timer0_counter == 0){
			timer0_flag = 1;
		}
		if(switch_led_time > 0){
			switch_led_time--;
			if(switch_led_time <= 0){
				switch_led_flag = 1;
			}
		}
	}
}

void updateClockBuffer(){
	if(hour >= 10){
		led_buffer[1] = hour%10;
		int temp = hour/10;
		led_buffer[0] = temp;
	}
	else{
		led_buffer[0] = 0;
		led_buffer[1] = hour;
	}

	if(minute >= 10){
		led_buffer[3] = minute%10;
		int temp = minute/10;
		led_buffer[2] = temp;
	}
	else{
		led_buffer[2] = 0;
		led_buffer[3] = minute;
	}
}

void update7SEG(int index){
	switch(index){
		case 0:
		{
			display7SEG(led_buffer[0]);
			break;
		}
		case 1:
		{
			display7SEG(led_buffer[1]);
			break;
		}
		case 2:
		{
			display7SEG(led_buffer[2]);
			break;
		}
		case 3:
		{
			display7SEG(led_buffer[3]);
			break;
		}
	}
}

void delay_half_of_sec(){
	counter--;
	if(counter <= 0){
		counter = 999;
	}
}

void display7SEG(int num){
	switch(num){
	case 0:
		GPIOB->ODR = 0xC0; //Displaying 0
		break;
	case 1:
		GPIOB->ODR = 0xf9; //Displaying 1
		break;
	case 2:
		GPIOB->ODR = 0xa4; //Displaying 2
		break;
	case 3:
		GPIOB->ODR = 0xb0; //Displaying 3
		break;
	case 4:
		GPIOB->ODR = 0x99; //Displaying 4
		break;
	case 5:
		GPIOB->ODR = 0x92; //Displaying 5
		break;
	case 6:
		GPIOB->ODR = 0x82; //Displaying 6
		break;
	case 7:
		GPIOB->ODR = 0xf8; //Displaying 7
		break;
	case 8:
		GPIOB->ODR = 0x80; //Displaying 8
		break;
	case 9:
		GPIOB->ODR = 0x90; //Displaying 9
		break;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  // set delay time here
  setTimer0(DELAY_TIME);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* ---------------------------------Clock---------------------------------------------*/
	  if(switch_led_flag == 1){
		  switch_led_flag = 0;

		  if(timer0_flag == 1){
			  calculate_time();
			  HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
			  setTimer0(DELAY_TIME);
		  }
		  show_clock();
		  switch_led_time = 25;
	  }
	  // comment that code block if you want to excute animation on LED matrix
	 /*-------------------------------------------------------------------------------------*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
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
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|DOT_Pin|EN2_Pin
                          |EN3_Pin|ENM0_Pin|ENM1_Pin|ENM2_Pin
                          |ENM3_Pin|ENM4_Pin|ENM5_Pin|ENM6_Pin
                          |ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : EN0_Pin EN1_Pin DOT_Pin LED_RED_Pin
                           EN2_Pin EN3_Pin ENM0_Pin ENM1_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = EN0_Pin|EN1_Pin|DOT_Pin|LED_RED_Pin
                          |EN2_Pin|EN3_Pin|ENM0_Pin|ENM1_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance)
	{
		timer_run();
		/*-------------------------------------------------------------------------------------------*/
//		if(column_scan > 7){
//			shift_left_to_right();
//			column_scan = 0;
//		}
//		updateLedMatrix(column_scan);
//		column_scan++;

		// uncomment that code block and comment fuction timer_run if you want to excute animation on LED matrix
		/*-------------------------------------------------------------------------------------------*/
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
