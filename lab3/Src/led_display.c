/* this file belong to lab3_exercise*/

#include "led_display.h"
#include "main.h"
#include "timer.h"
#include "input_reading.h"

#define 		TIME_DURATION_OF_RED_LIGHT 				7
#define			TIME_DELAY_PER_COUNTDOWN				50

//int counterForRedDuration = 0;
int counterForPerCountdown = 0;
int timeDuration = 7;
int timeDurationCountdown = 0;
int durationCounter = 0;
int red_duration = 0;
int yellow_duration = 0;
int green_duration = 0;
int led_buffer[4] = {0,0,0,0};
const int MAX_LED = 4;
int index_led = 0;

typedef enum{
	WAY1_AND_WAY3_RED,
	WAY2_AND_WAY4_RED
}TRAFFIC_LIGHT_STATE;
TRAFFIC_LIGHT_STATE traffic_light_state = WAY2_AND_WAY4_RED;

typedef enum{
	LED1_ON,
	LED2_ON,
	LED3_ON,
	LED4_ON
}LED_STATE;
LED_STATE led_state = LED1_ON;

void calculate_mode_and_timeDuration(){
	durationCounter++;
	if(durationCounter > 99)
		durationCounter = 0;
	update_mode_and_timeDuration_buffer();
}

void update_mode_and_timeDuration_buffer(){
	led_buffer[0] = 0;
	led_buffer[1] = mode;
	if(durationCounter>= 10){
		led_buffer[2] = durationCounter/10;
		led_buffer[3] = durationCounter%10;
	}
	else{
		led_buffer[2] = 0;
		led_buffer[3] = durationCounter;
	}
}

void display_mode_and_timeDuration(){
	if(switch_led_flag == 1){
		switch_led_flag = 0;
		if(timer0_flag == 1){
			setTimer0(DELAY_TIME);
		}
		show_clock();
		switch_led_time = 25;
	}
}


void show_countdown_7segLed_green_light(){
	timeDurationCountdown = green_duration;
	while(timeDurationCountdown > 0){
		if(modeButtonPressedFlag)
			break;
		if(switch_led_flag == 1){
		  switch_led_flag = 0;

		  if(timer0_flag == 1){
			  calculate_time();
			  setTimer0(DELAY_TIME);
		  }
		  show_clock();
		  switch_led_time = 25;
		}
	}
}

void show_countdown_7segLed_yellow_light(){
	timeDurationCountdown = yellow_duration;
	while(timeDurationCountdown > 0){
		if(modeButtonPressedFlag)
			break;
		if(switch_led_flag == 1){
		  switch_led_flag = 0;

		  if(timer0_flag == 1){
			  calculate_time();
			  setTimer0(DELAY_TIME);
		  }
		  show_clock();
		  switch_led_time = 25;
		}
	}
}

void updateClockBuffer(){
	if(timeDurationCountdown>= 10){
		led_buffer[1] = timeDurationCountdown%10;
		led_buffer[3] = timeDurationCountdown%10;
		int temp = timeDurationCountdown/10;
		led_buffer[0] = temp;
		led_buffer[2] = temp;
	}
	else{
		led_buffer[0] = 0;
		led_buffer[2] = 0;
		led_buffer[1] = timeDurationCountdown;
		led_buffer[3] = timeDurationCountdown;
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

void calculate_time(){
	timeDurationCountdown--;
	updateClockBuffer();
}

void show_clock(){
	switch(led_state){
		case LED1_ON:
		{
			update7SEG(0);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			//display7SEG(led_buffer[0]);
			led_state = LED2_ON;
			break;
		}
		case LED2_ON:
		{
			update7SEG(1);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			//display7SEG(led_buffer[1]);
			led_state = LED3_ON;
			break;
		}
		case LED3_ON:
		{
			update7SEG(2);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, RESET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			//display7SEG(led_buffer[2]);
			led_state = LED4_ON;
			break;
		}
		case LED4_ON:
		{
			update7SEG(3);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, RESET);
			//display7SEG(led_buffer[3]);
			led_state = LED1_ON;
			break;
		}
	}
}

void traffic_light_fsm(){
	switch(traffic_light_state){
		case WAY2_AND_WAY4_RED:
		{
		    /*                    WAY 1 and WAY 3                           */
			// Green Led of way1 and way3 are high and red led of all other ways are high for 3 seconds
			HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RED3_GPIO_Port, RED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GREEN3_GPIO_Port, GREEN3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RED4_GPIO_Port, RED4_Pin, GPIO_PIN_SET);
			show_countdown_7segLed_for_green_light();
			// Yellow Led of way1 and way3 are high and red led of all other ways are high for 2 seconds
			HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YELLOW1_GPIO_Port, YELLOW1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GREEN3_GPIO_Port, GREEN3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YELLOW3_GPIO_Port, YELLOW3_Pin, GPIO_PIN_SET);
			show_countdown_7segLed_for_yellow_light();
			HAL_GPIO_WritePin(YELLOW1_GPIO_Port, YELLOW1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YELLOW3_GPIO_Port, YELLOW3_Pin, GPIO_PIN_RESET);
			traffic_light_state = WAY1_AND_WAY3_RED;
			break;
		}
		case WAY1_AND_WAY3_RED:
		{
		    /*                    WAY 2 and WAY 4                             */
			// Green Led of way2 and way4 are high and red led of all other ways are high for 3 seconds
			HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RED4_GPIO_Port, RED4_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RED3_GPIO_Port, RED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GREEN4_GPIO_Port, GREEN4_Pin, GPIO_PIN_SET);
			show_countdown_7segLed_for_green_light();
			// Yellow Led of way2 and way4 high and red led of all other ways are high for 2 seconds
			HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YELLOW2_GPIO_Port, YELLOW2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GREEN4_GPIO_Port, GREEN4_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YELLOW4_GPIO_Port, YELLOW4_Pin, GPIO_PIN_SET);
			show_countdown_7segLed_for_yellow_light();
			HAL_GPIO_WritePin(YELLOW2_GPIO_Port, YELLOW2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YELLOW4_GPIO_Port, YELLOW4_Pin, GPIO_PIN_RESET);
			traffic_light_state = WAY2_AND_WAY4_RED;
			break;
		}
	}
}

void display_countdown(){
	int counter = TIME_DURATION_OF_RED_LIGHT;
	while(counter > 0){
		counterForPerCountdown = TIME_DELAY_PER_COUNTDOWN;
		display7SEG(counter);
		counter = counter - 1;
		while(counterForPerCountdown > 0){
		}
	}
}

void display7SEG(int num){
	switch(num){
	case 0:
		GPIOB->ODR = 0xc0ff; //Displaying 0
		break;
	case 1:
		GPIOB->ODR = 0xf9ff; //Displaying 1
		break;
	case 2:
		GPIOB->ODR = 0xa4ff; //Displaying 2
		break;
	case 3:
		GPIOB->ODR = 0xb0ff; //Displaying 3
		break;
	case 4:
		GPIOB->ODR = 0x99ff; //Displaying 4
		break;
	case 5:
		GPIOB->ODR = 0x92ff; //Displaying 5
		break;
	case 6:
		GPIOB->ODR = 0x82ff; //Displaying 6
		break;
	case 7:
		GPIOB->ODR = 0xf8ff; //Displaying 7
		break;
	case 8:
		GPIOB->ODR = 0x80ff; //Displaying 8
		break;
	case 9:
		GPIOB->ODR = 0x90ff; //Displaying 9
		break;
	}
}



