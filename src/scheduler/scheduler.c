/*
 * scheduler.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */
#include "scheduler.h"



timer_scheduler_t systick_timer;
uint16_t ms_delay_counter=0U;

void scheduler_main(void);
uint8_t check_ms_counter(void);
void reset_ms_counter(void);
/**===========================================================================
**
**  Abstract: SysTick interrupt handler
**
**===========================================================================
*/

/* Should be 1ms period, check by debug pin*/

void SysTick_Handler(void)
{
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

void systic_delay_ms(uint16_t delay_ms_count)
{
	reset_ms_counter();
	for(;delay_ms_count <ms_delay_counter;)
	{
		__ASM volatile ("nop");
	}
}

uint8_t check_ms_counter(void)
{
	return ms_delay_counter;
}

void reset_ms_counter(void)
{
	ms_delay_counter = 0;
}

void systick_init(void)
{
	/*SET to 1ms period*/
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick->LOAD  = 16000;      									 /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
    SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */

    systick_timer.timer_1000ms = 0;
    systick_timer.timer_100ms = 0;
    systick_timer.timer_10ms = 0;
}


