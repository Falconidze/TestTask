#include "stdint.h"
#include "stdbool.h"
#include "timer_and_control.h"
#include "display.h"
#include "gpio.h"
#include "math.h"

static callback_func_t callback_func_ptr;
static uint32_t timer_total_seconds;

static timer_state_e_t timer_state_e;
static bool is_relay_open = false;

void timer_stop();
void timer_start();


callback_func_t get_callback()
{
	return callback_func_ptr;
}

void relay_open()
{
	GPIOA->ODR |= GPIO_ODR_15;
	is_relay_open = true;
}

void relay_close()
{
	GPIOA->ODR &= ~GPIO_ODR_15;
	is_relay_open = false;
}

void timer_stop()
{
	if(is_relay_open)
	{
		relay_close();
	}
	timer_state_e = Stopped;
}

uint32_t time_to_number()
{
	uint32_t minutes = timer_total_seconds / 60;
	uint32_t seconds = timer_total_seconds % 60;
	return (uint32_t)((minutes * pow(10, 2)) + seconds);
}

void timer_start()
{
	timer_total_seconds = 60;
	change_displayed_number(time_to_number());
	callback_func_ptr = &timer_stop;
	timer_state_e = Active;
}

void init_timer_and_control()
{
	timer_total_seconds = 60;
	timer_state_e = Stopped;
	callback_func_ptr = &timer_start;
}

void timer_tick_down()
{
	timer_total_seconds--;	
}

void timer_work()
{
	if(timer_state_e == Active)
	{
		timer_tick_down();
		change_displayed_number(timer_total_seconds);
		if(timer_total_seconds == 0)
		{
			relay_open();

			timer_total_seconds = 3;
			callback_func_ptr = &timer_start;			
			timer_state_e = Finished;
		}
	}
	else if(timer_state_e == Finished)
	{
		timer_tick_down();
		if(timer_total_seconds == 0)
		{
			relay_close();
			timer_state_e = Stopped;
			timer_total_seconds = 60;
		}
	}
}
