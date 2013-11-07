/*
 * init.h
 *
 *  Created on: Jun 26, 2013
 *      Author: Bono
 */

#ifndef INIT_H_
#define INIT_H_

#include <stdint.h>
#include "stm32f0xx_misc.h"
#include "scheduler.h"
#include "ST7565R.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "dds.h"

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


extern void init_devices(void);


#endif /* INIT_H_ */
