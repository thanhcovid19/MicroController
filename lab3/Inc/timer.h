/* this file belong to lab3_exercise*/

#ifndef INC_TIMER_H
#define INC_TIMER_H

extern int timer0_flag;
extern int switch_led_flag;
extern int switch_led_time;
extern int DELAY_TIME;

extern void setTimer0(int duration);
void timer_run();

#endif /*INC_LED_DISPLAY_H*/
