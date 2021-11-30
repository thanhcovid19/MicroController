/* this file belong to lab3_exercise*/

#ifndef INC_INPUT_READING_H
#define INC_INPUT_READING_H

typedef enum{
	NORMAL,
	MODIFY_TIME_DURATION_RED,
	MODIFY_TIME_DURATION_YELLOW,
	MODIFY_TIME_DURATION_GREEN
} MODE;

extern MODE mode;

extern int modeButton;
extern int modifyButton;
extern int setButton;
extern int modeButtonPressedFlag;
extern int modifyButtonPressedFlag;
extern int setButtonPressedFlag;

void button_reading();
extern void initial_value_for_flags_and_counters();

#endif /* INC_INPUT_READING_H */
