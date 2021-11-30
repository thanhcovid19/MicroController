/* this file belong to lab3_exercise*/

#include "main.h"
#include "input_processing.h"
#include "led_display.h"
#include "input_reading.h"

int counterForBlinkTime = -1;
int blinkCycle = 50;
int blinkFlag = 0;
int modifyModeFlag = 0;
int redModificationIsConfirmed = 0;
int yellowModificationIsConfirmed = 0;
int greenModificationIsConfirmed = 0;

void turn_off_all_led(){
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, RESET);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, RESET);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED3_Pin, RESET);
	HAL_GPIO_WritePin(RED4_GPIO_Port, RED4_Pin, RESET);

	HAL_GPIO_WritePin(YELLOW1_GPIO_Port, YELLOW1_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW2_GPIO_Port, YELLOW2_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW3_GPIO_Port, YELLOW3_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW4_GPIO_Port, YELLOW4_Pin, RESET);

	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, RESET);
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, RESET);
	HAL_GPIO_WritePin(GREEN3_GPIO_Port, GREEN3_Pin, RESET);
	HAL_GPIO_WritePin(GREEN3_GPIO_Port, GREEN4_Pin, RESET);
}

void blink_red_light(){
	if(blinkFlag){
		HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
		HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
		HAL_GPIO_TogglePin(RED3_GPIO_Port, RED3_Pin);
		HAL_GPIO_TogglePin(RED4_GPIO_Port, RED4_Pin);
		blinkFlag = 0;
		counterForBlinkTime = 0;
	}
}

void blink_yellow_light(){
	if(blinkFlag){
		HAL_GPIO_TogglePin(YELLOW1_GPIO_Port, YELLOW1_Pin);
		HAL_GPIO_TogglePin(YELLOW2_GPIO_Port, YELLOW2_Pin);
		HAL_GPIO_TogglePin(YELLOW3_GPIO_Port, YELLOW3_Pin);
		HAL_GPIO_TogglePin(YELLOW4_GPIO_Port, YELLOW4_Pin);
		blinkFlag = 0;
		counterForBlinkTime = 0;
	}
}

void blink_green_light(){
	if(blinkFlag){
		HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);
		HAL_GPIO_TogglePin(GREEN2_GPIO_Port, GREEN2_Pin);
		HAL_GPIO_TogglePin(GREEN3_GPIO_Port, GREEN3_Pin);
		HAL_GPIO_TogglePin(GREEN4_GPIO_Port, GREEN4_Pin);
		blinkFlag = 0;
		counterForBlinkTime = 0;
	}
}

void confirm_red_modify(){
	// turn off blink red light
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, RESET);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, RESET);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED3_Pin, RESET);
	HAL_GPIO_WritePin(RED4_GPIO_Port, RED4_Pin, RESET);
	red_duration = durationCounter;
	durationCounter = 0;
	counterForBlinkTime = -1;
	blinkFlag = 0;
	redModificationIsConfirmed = 1;
}

void confirm_yellow_modify(){
	// turn off blink red light
	HAL_GPIO_WritePin(YELLOW1_GPIO_Port, YELLOW1_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW2_GPIO_Port, YELLOW2_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW3_GPIO_Port, YELLOW3_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW4_GPIO_Port, YELLOW4_Pin, RESET);
	yellow_duration = durationCounter;
	durationCounter = 0;
	counterForBlinkTime = -1;
	blinkFlag = 0;
	yellowModificationIsConfirmed = 1;
}

void confirm_green_modify(){
	// turn off blink red light
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, RESET);
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, RESET);
	HAL_GPIO_WritePin(GREEN3_GPIO_Port, GREEN3_Pin, RESET);
	HAL_GPIO_WritePin(GREEN3_GPIO_Port, GREEN4_Pin, RESET);
	green_duration = durationCounter;
	durationCounter = 0;
	counterForBlinkTime = -1;
	blinkFlag = 0;
	greenModificationIsConfirmed = 1;

}

void modify_red_duration_time(){
	if(counterForBlinkTime == -1)         // turn off all red led before blink
		turn_off_all_led();
	modifyModeFlag = 1;                   // stop operation in normal mode
	if(counterForBlinkTime == -1  && !redModificationIsConfirmed)
		counterForBlinkTime = 0;          // start counter for blink red lights
	blink_red_light();
}

void modify_yellow_duration_time(){
	if(counterForBlinkTime == -1)         // turn off all yellow led before blink
		turn_off_all_led();
	modifyModeFlag = 1;                   // stop operation in normal mode
	if(counterForBlinkTime == -1 && !yellowModificationIsConfirmed)
		counterForBlinkTime = 0;          // start counter for blink red lights
	blink_yellow_light();
}

void modify_green_duration_time(){
	if(counterForBlinkTime == -1)         // turn off all green led before blink
		turn_off_all_led();
	modifyModeFlag = 1;                   // stop operation in normal mode
	if(counterForBlinkTime == -1 && !greenModificationIsConfirmed)
		counterForBlinkTime = 0;          // start counter for blink red lights
	blink_green_light();
}

void fsm_for_input_processing(){
	switch(mode){
		case NORMAL:
		{
			redModificationIsConfirmed = 0;
			yellowModificationIsConfirmed = 0;
			greenModificationIsConfirmed = 0;
			if(modifyModeFlag){
				turn_off_all_led();
			}
			modifyModeFlag = 0;
			traffic_light_fsm();
			if(modeButtonPressedFlag){
				mode = MODIFY_TIME_DURATION_RED;
				modeButtonPressedFlag = 0;
			}
			break;
		}
		case MODIFY_TIME_DURATION_RED:
		{
			modify_red_duration_time();
			display_mode_and_timeDuration();
			if(modifyButtonPressedFlag){
				calculate_mode_and_timeDuration();
				modifyButtonPressedFlag = 0;
			}
			if(setButtonPressedFlag){
				confirm_red_modify();
				setButtonPressedFlag = 0;
				mode = NORMAL;
			}
			if(modeButtonPressedFlag){
				mode = MODIFY_TIME_DURATION_YELLOW;
				modeButtonPressedFlag = 0;
			}
			break;
		}
		case MODIFY_TIME_DURATION_YELLOW:
		{
			modify_yellow_duration_time();
			if(modifyButtonPressedFlag){
				display_mode_and_timeDuration();
				modifyButtonPressedFlag = 0;
			}
			if(setButtonPressedFlag){
				confirm_yellow_modify();
				setButtonPressedFlag = 0;
				mode = NORMAL;
			}
			if(modeButtonPressedFlag){
				mode = MODIFY_TIME_DURATION_GREEN;
				modeButtonPressedFlag = 0;
			}
			break;
		}
		case MODIFY_TIME_DURATION_GREEN:
		{
			modify_green_duration_time();
			if(modifyButtonPressedFlag){
				display_mode_and_timeDuration();
				modifyButtonPressedFlag = 0;
			}
			if(setButtonPressedFlag){
				confirm_green_modify();
				setButtonPressedFlag = 0;
				mode = NORMAL;
			}
			if(modeButtonPressedFlag){
				mode = NORMAL;
				modeButtonPressedFlag = 0;
			}
			break;
		}
	}
}
