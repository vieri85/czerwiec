/*
 * scheduler.h
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include "task_functions.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


typedef struct
{
	uint8_t timer_10ms;
	uint8_t timer_100ms;
	uint16_t timer_1000ms;
}timer_scheduler_t;


typedef enum
{
	DELAY_DONE,
	DELAY_PENDING,
	DELAY_IDLE

}del_ms_state_t;

typedef enum
{
	DELAY_START,
	DELAY_STATE
}del_cmd_t;

extern timer_scheduler_t systick_timer;



extern void systic_delay_ms(uint16_t delay_ms_count);


#endif /* SCHEDULER_H_ */
