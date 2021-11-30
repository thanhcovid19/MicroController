/* this file belong to lab3_exercise*/

#include "main.h"
#include "input_reading.h"
// we aim to work with more than one buttons
#define 		NO_OF_BUTTON								3
// timer interrupt duration is 10ms, so to pass 1 second,
// we need to jump to the interrupt service routine 100 times

#define 		BUTTON_IS_PRESSED							GPIO_PIN_RESET
#define 		BUTTON_IS_REALEASED							GPIO_PIN_SET

MODE mode = NORMAL;

int modeButton = 0;
int modifyButton = 1;
int setButton = 2;

int modeButtonPressedFlag = 0;
int modifyButtonPressedFlag = 0;
int setButtonPressedFlag = 0;
int isExcuted = 0;

//the buffer that the final result is stored after de-bouncing
//static GPIO_PinState buttonBuffer[NO_OF_BUTTON];
//
//// we define two
//static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTON];
//static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTON];

static GPIO_PinState buttonBuffer[NO_OF_BUTTON];

// we define two
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTON];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTON];



void initial_value_for_flags_and_counters(){
	for(int i = 0; i < NO_OF_BUTTON; i++){
		debounceButtonBuffer1[i] = BUTTON_IS_REALEASED;
		debounceButtonBuffer2[i] = BUTTON_IS_REALEASED;
	}
}

void button_reading(){
	for(int i = 0; i < NO_OF_BUTTON; i++){
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		if(i == modeButton){
			debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(MODE_BUTTON_GPIO_Port, MODE_BUTTON_Pin);
		}
		if(i == modifyButton){
			debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(MODIFY_BUTTON_GPIO_Port, MODIFY_BUTTON_Pin);
		}
		if(i == setButton){
			debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(SET_BUTTON_GPIO_Port, SET_BUTTON_Pin);
		}

		if(debounceButtonBuffer1[i] != debounceButtonBuffer2[i]){
			buttonBuffer[i] = debounceButtonBuffer1[i];
			// if button is pressed, we start counting
			if(buttonBuffer[i] == 0){
				//flagForButtonPressed[i] = 1;
				if(i == modeButton){
					modeButtonPressedFlag = 1;
				}
				if(i == modifyButton){
					modifyButtonPressedFlag = 1;
				}
				if(i == setButton){
					setButtonPressedFlag = 1;
				}
			}
		}
	}
}
