/*
 * task_functions.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#include "task_functions.h"
#include "uart_functions.h"
#include "timer.h"
#include "dds.h"

uint8_t timer_flag_1s=0;



void number(line_operation_t *line_operation_ptr);


void task_every_1ms(void)
{

	//SPI_SendData8(SPI1, 255);

}

void task_every_10ms(void)
{
	uint8_t q;
	uint8_t a;
	uart_task();
	for(q=20;q>0;q--)
	{

			GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_0);
			delay_us(100);
			GPIO_SetBits(DEBUG_PORT, DEBUG_PIN_0);
		    delay_us(100);
	}


}

void task_every_100ms(void)
{
	menu_main();


}

void task_every_1000ms(void)
{
	static uint32_t freq=1;


	freq++;

		timer_flag_1s = 1;
		//number(&line_operation_1);
		//uart_send_word(read_encoder());

		set_frequency(freq);
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

