/*
 * common_use.h
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#ifndef COMMON_USE_H_
#define COMMON_USE_H_

#include "inttypes.h"
#include "st7565r.h"

/* STM32F0_DISCOVERY board definitions */
#define LED_PORT GPIOC
#define LED1 GPIO_Pin_9
#define LED2 GPIO_Pin_8
#define DEBUG_PIN_0 GPIO_Pin_0
#define DEBUG_PIN_1 GPIO_Pin_1

#define KEY_PORT GPIOA
#define KEY GPIO_Pin_0
#define DEBUG_PORT GPIOB
/* Private macro */

#define BIT00	(1<<0)
#define BIT01	(1<<1)
#define BIT02	(1<<2)
#define BIT03	(1<<3)
#define BIT04	(1<<4)
#define BIT05	(1<<5)
#define BIT06	(1<<6)
#define BIT07	(1<<7)
#define BIT08	(1<<8)
#define BIT09	(1<<9)
#define BIT10	(1<<10)
#define BIT11	(1<<11)
#define BIT12	(1<<12)
#define BIT13	(1<<13)
#define BIT14	(1<<14)
#define BIT15	(1<<15)


extern uint8_t BCD_TO_DEC(uint8_t* bufor,uint8_t *dec);
extern void delay (int a);
extern uint8_t letter_check(void);
extern uint8_t new_receive_check(void);
extern void new_receive_set(uint8_t value);
extern void set_debug_pin(uint16_t debug_pin_nr);
extern void reset_debug_pin(uint16_t debug_pin_nr);
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
#endif /* COMMON_USE_H_ */
