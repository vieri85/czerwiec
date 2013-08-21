/*
 * task_functions.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#include "task_functions.h"
#include "uart_functions.h"

uint8_t timer_flag_1s=0;



void number(line_operation_t *line_operation_ptr);


void task_every_1ms(void)
{
	if(0 != (DEBUG_PORT->ODR & DEBUG_PIN_1))
	{
		GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);
	}
	else
	{
		GPIO_SetBits(DEBUG_PORT, DEBUG_PIN_1);
	}
	//SPI_SendData8(SPI1, 255);
}

void task_every_10ms(void)
{
	;

}

void task_every_100ms(void)
{
	menu_opertion();
	//menu_main();
	uart_operation_for_LCD();
	//menu_line_put(line_operation_1);
}

void task_every_1000ms(void)
{
	static uint8_t dec_number=0;

	dec_number++;

		timer_flag_1s = 1;
		number(&line_operation_1);

}




void reset_timerFlag(void)
{
	timer_flag_1s = 0;
}

uint8_t check_timerFlag(void)
{
	return timer_flag_1s;
}

void number(line_operation_t *line_operation_ptr)
{
	static uint8_t tab[2]={0,0};
	static uint8_t licz=48;
	if(licz<58)
	{
		++licz;
	}
	else
		licz=48;

	tab[0]=licz;
	line_operation_ptr->sentence_med = tab;
}

