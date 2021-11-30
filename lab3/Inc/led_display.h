/* this file belong to lab3_exercise*/

#ifndef INC_LED_DISPLAY_H
#define INC_LED_DISPLAY_H

extern int counterForPerCountdown;
extern int durationCounter;
extern int red_duration;
extern int yellow_duration;
extern int green_duration;

void display7SEG(int num);
void display_countdown();
void traffic_light_fsm();
void red_countdown();
void show_countdown_7segLed_for_green_light();
void show_countdown_7segLed_for_yellow_light();
extern void calculate_time();
void updateClockBuffer();
void update7SEG(int index);
extern void show_clock();
extern void display_mode_and_timeDuration();
extern void calculate_mode_and_timeDuration();
void update_mode_and_timeDuration_buffer();

#endif /*INC_LED_DISPLAY_H*/
