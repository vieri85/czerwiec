/*
 * scheduler.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */
#include "scheduler.h"
#include "common_use.h"

uint32_t timer=0;

/**===========================================================================
**
**  Abstract: SysTick interrupt handler
**
**===========================================================================
*/

/* Should be 1ms period, check by debug pin*/

void SysTick_Handler(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB,DEBUG_PIN_0))
	reset_debug_pin(DEBUG_PIN_0);
	else
    set_debug_pin(DEBUG_PIN_0);

	timer++;
	if(timer>1000)
	{
		USART_SendData(USART2, 0x67);
		timer=0;
	}

}

/* ToBeDone
void scheduler_main(void)
{

	if(event_count_u8&1)
	{
		task_0();
		event_count_u8&=~1;
	}

	if(event_count_u8&2)
	{
		task_1();
		event_count_u8&=~2;
	}


	if(event_count_u8&4)
	{
		task_2();
		event_count_u8&=~4;
	}
}

void systic_perform(void)
{
	if(sched_au16[0]==tim_100us_u16)
	{
		event_count_u8|=1;
	}

	if(sched_au16[1]==tim_100us_u16)
	{
		event_count_u8|=2;
	}

	if(sched_au16[2]==tim_100us_u16)
	{
		event_count_u8|=4;
	}
++tim_100us_u16;
}
*/
