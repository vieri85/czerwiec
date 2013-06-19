/*
 * task_functions.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#include "task_functions.h"


void task_every_1ms(void)
{

}

void task_every_10ms(void)
{

}

void task_every_100ms(void)
{
	led_blinkin();
}

void task_every_1000ms(void)
{
	static uint8_t dec_number=0;
	dec_number++;
	ST7565R_Write(dat, dec_number);
}
