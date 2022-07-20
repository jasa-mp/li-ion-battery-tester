/* rtos_tasks.h */

// header guard
#ifndef RTOS_TASKS_H    
#define RTOS_TASKS_H    

// include

#include <rtos.h>

// declarations
#define TASK_COUNT 1

// tasks function prototypes
void stopwatch(void);           // keeps track of time
void battery_checker(void);     // checks battery voltage and battery load current 
void button_status(void);       // checks if buttons are pressed and reacts accordingly
void write_to_oled(void);       // writes all the necessary information to OLED
void blink_led(void);           // testing RTOS

#endif 