#include "stdbool.h"
#include "stdint.h"
#include "button.h"
#include "gpio.h"

#define debounce_time_count 5

static bool button_is_pressed = false;
static uint8_t count = 0;


void button_check(void (*callback)())
{	
	{
		if (!(GPIOC->IDR & GPIO_IDR_13) && (button_is_pressed == false))
		{
			count++;
			if(count == debounce_time_count)
			{
				count = 0;
				button_is_pressed = true;
				(*callback)();
			}
		}
		else if ((GPIOC->IDR & GPIO_IDR_13) && (button_is_pressed == true))
		{
			count++;
			if(count == debounce_time_count)
			{
				count = 0;
				button_is_pressed = false;
			}
		}
	}
}