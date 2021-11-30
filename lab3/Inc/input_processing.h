/* this file belong to lab3_exercise*/

#ifndef INC_PROCESSING_H
#define INC_PROCESSING_H

extern int counterForBlinkTime;
extern int blinkCycle;
extern int blinkFlag;
extern int modifyModeFlag;

void fsm_for_input_processing();
void modify_red_duration_time();
void modify_yellow_duration_time();
void modify_green_duration_time();
void blink_red_light();
void blink_yellow_light();
void blink_green_light();
void confirm_red_modify();
void confirm_yellow_modify();
void confirm_green_modify();
void turn_off_all_led();

#endif /*INC_PROCESSING_H*/
