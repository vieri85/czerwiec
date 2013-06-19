/*
 * scheduler.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */
#include "scheduler.h"
#include "common_use.h"



timer_scheduler_t systick_timer;


void scheduler_main(void);

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

	scheduler_main();
}

void scheduler_main(void)
{
	 ++systick_timer.timer_1000ms;
	 ++systick_timer.timer_100ms;
	 ++systick_timer.timer_10ms;

	 task_every_1ms();

	 if(systick_timer.timer_10ms>=10)
	 {
		 task_every_10ms();
		 systick_timer.timer_10ms = 0;
	 }

	 if(systick_timer.timer_100ms>=100)
	 {
		 task_every_100ms();
		 systick_timer.timer_100ms = 0;
	 }

	 if(systick_timer.timer_1000ms>=1000)
	 {
		 task_every_1000ms();
		 systick_timer.timer_1000ms = 0;
	 }



}



