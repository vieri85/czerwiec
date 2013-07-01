/*
 * scheduler.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */
#include "scheduler.h"
#include "common_use.h"



timer_scheduler_t systick_timer;
uint32_t ms_delay_counter=0U;

void scheduler_main(void);
uint32_t check_ms_counter(void);
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
	 ++ms_delay_counter;

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

void systic_delay_ms(uint32_t delay_ms_count)
{
	uint32_t fin_counter_u32;

	fin_counter_u32 = delay_ms_count + check_ms_counter();
	while(fin_counter_u32 != check_ms_counter())
	{
		;
	}
}

uint32_t check_ms_counter(void)
{
	return ms_delay_counter;
}
