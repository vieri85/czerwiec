/*
 * dds.h
 *
 *  Created on: Oct 24, 2013
 *      Author: Bono
 */

#ifndef DDS_H_
#define DDS_H_
#include "stdint.h"
#include "system_stm32f0xx.h"
#include "stm32f0xx_gpio.h"

#define DDS_QUARC_VALUE 125000000UL
#define DDS_FACTOR		4294967296UL

#define DDS_W_CLK			GPIO_Pin_4
#define DDS_FQ_UD			GPIO_Pin_5
#define DDS_SERIAL_DATA 	GPIO_Pin_6

extern void delay_us(uint16_t delay_us);
extern void set_frequency(uint32_t frequency_32);
extern void init_DDS(void);

#endif /* DDS_H_ */
