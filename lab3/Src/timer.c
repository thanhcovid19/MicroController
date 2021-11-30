/* this file belong to lab3_exercise*/

#include "timer.h"
#include "input_processing.h"

int DELAY_TIME = 500;
int timer0_flag = 0;
int switch_led_flag = 0;
int switch_led_time = 25;
int timer0_counter = 0;
int TIMER_CYCLE = 10;

void setTimer0(int duration){
	timer0_counter = duration/TIMER_CYCLE;
	timer0_flag = 0;
}

void timer_run(){
	if(counterForBlinkTime >= 0){
		counterForBlinkTime++;
		if(counterForBlinkTime == blinkCycle){
			blinkFlag = 1;
		}
	}
	if(timer0_counter > 0){
		timer0_counter--;
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
