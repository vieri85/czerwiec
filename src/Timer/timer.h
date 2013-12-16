/*
 * timer.h
 *
 *  Created on: Sep 30, 2013
 *      Author: Bono
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef enum
{
	INCREMENT= 0,
	DECREMENT,
	STABLE
}EnkoderState_t;

extern uint16_t read_encoder(void);
extern void set_encoder(uint16_t new_value);

#endif /* TIMER_H_ */
