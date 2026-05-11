#ifndef __TIMERS_H
#define __TIMERS_H

#include "stdint.h"

typedef void (*callback_func_t)(void);
typedef enum 
{
	Stopped,
	Active,
	Finished
}timer_state_e_t;	
	
void init_timer_and_control();
uint32_t time_to_number();
void timer_work();
callback_func_t get_callback();

#endif