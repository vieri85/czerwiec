/*
 * keyboard.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Bono
 */
#include "keyboard.h"

static switch_t global_button_state;

void Button_Set(switch_t switch_nr)
{
	global_button_state = switch_nr;
}


switch_t read_any_button(void)
{
	switch_t ret;
   ret = global_button_state;
   global_button_state = 0;

   return ret;
}

void check_button(void)
{
	if(0 == GPIO_ReadInputDataBit(KEYBOARD_PORT, KEYBOARD_ENTER))
	{
		Button_Set(ENTER);
	}
	else if(0 == GPIO_ReadInputDataBit(KEYBOARD_PORT, KEYBOARD_ESC))
	{
		Button_Set(ESC);
	}
	else if(0 == GPIO_ReadInputDataBit(KEYBOARD_PORT, KEYBOARD_LEFT))
	{
		Button_Set(LEFT);
	}
	else if(0 == GPIO_ReadInputDataBit(KEYBOARD_PORT, KEYBOARD_RIGHT))
	{
		Button_Set(RIGHT);
	}
}
