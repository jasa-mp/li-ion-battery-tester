/* rtos.h */

// header guard
#ifndef RTOS_H    
#define RTOS_H    

// include
#include <stdint.h>

// declarations
// CPU CLK = F_CPU

#define F_CPU_MH 16


#define RTOS_SLICE_US 10000     // 10 ms
// max RTOS slice = 32,7 ms with this setup

// function pointer
typedef void (*ptr_function)(void);

// task struct
struct _task{
    char name[20];
    uint32_t last_tick;
    ptr_function fun;
}; 
typedef struct _task rtos_task_t;


// function prototypes 
uint32_t rtos_init(void);
void rtos_enable(void);
void rtos_disable(void);

#endif 