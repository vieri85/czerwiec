/*
 * task_functions.h
 *
 *  Created on: 19-06-2013
 *      Author: Bono
 */

#ifndef TASK_FUNCTIONS_H_
#define TASK_FUNCTIONS_H_
#include <inttypes.h>
#include "init.h"
#include "ST7565R.h"

extern void task_every_1ms(void);
extern void task_every_10ms(void);
extern void task_every_100ms(void);
extern void task_every_1000ms(void);
extern uint8_t check_timerFlag(void);
extern void reset_timerFlag(void);

#endif /* TASK_FUNCTIONS_H_ */
