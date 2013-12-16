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
#include "keyboard.h"
#include "menu.h"

uint8_t timer_flag_1s=0;
static uint16_t enk_old=0;
uint16_t licznik=0;


void number(line_operation_t *line_operation_ptr);


void task_every_1ms(void)
{

	uart_task();

}

void task_every_10ms(void)
{



}

void task_every_100ms(void)
{


	menu_main();
	check_button();
//	if(enk_old != read_encoder())
//	{
//		enk_old = read_encoder();
//		(void)uart_write_buf(&enk_old, sizeof(enk_old));
//	}


}

void task_every_1000ms(void)
{
	static uint32_t freq=1;
	static uint8_t ii2=0;

	freq++;

		timer_flag_1s = 1;



//		set_frequency(freq);
				  if (ii2 == 1)
				  {

					  LED_PORT->BSRR = LED1;
					  ii2 = 0;

				  }
				  else if (ii2 == 0)
				  {
					  ii2 = 1;
					  LED_PORT->BRR = LED1;


				  }
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

