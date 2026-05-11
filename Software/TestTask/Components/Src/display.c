#include "stdint.h"
#include "display.h"
#include "stdbool.h"
#include "gpio.h"

#define digit_length 3
#define digit_mask (GPIO_ODR_10 | GPIO_ODR_11 | GPIO_ODR_12)
#define segment_mask 0x00FF

static uint32_t displayed_number;
static uint8_t current_digit;
static uint32_t seg_out_array[digit_length];

static bool is_colon_on;

static uint32_t seg_digits_array[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void init_display()
{
	for(uint8_t i = 0; i < digit_length; i++)
	{
		seg_out_array[i] = 0;	
	}
	current_digit = 0;
	is_colon_on = true;
}

void set_out_array()
{
	uint32_t displayed_number = displayed_number;
	for (int8_t i = (sizeof(seg_out_array) / sizeof(uint32_t)) - 1; i >= 0; i--)
	{		
		seg_out_array[i] = displayed_number % 10;
		displayed_number /= 10;
	}
}

void change_displayed_number(uint32_t number)
{
	displayed_number = number;
	set_out_array();
}

void digit_on(uint8_t digit)
{	
	switch(digit)
	{
		case(0):
		GPIOA->ODR |= GPIO_ODR_10;
		break;
		
		case(1):
		GPIOA->ODR |= GPIO_ODR_11;
		break;
		
		case(2):
		GPIOA->ODR |= GPIO_ODR_12;
		break;			
	}
}

void digits_off()
{
	GPIOA->ODR &= ~digit_mask;
}

void output_symbol()
{
	digits_off();
	uint8_t digit = seg_out_array[current_digit];
	
	GPIOA->ODR &= ~segment_mask;	
	GPIOA->ODR |= seg_digits_array[digit];
	if((is_colon_on) && (current_digit != 2))
	{
		GPIOA->ODR |= GPIO_ODR_7; 
	}
	digit_on(current_digit);
}

void display_work()
{
	output_symbol();
	current_digit ++;
	if(current_digit > 2) 
	{
		current_digit = 0;
	}
}