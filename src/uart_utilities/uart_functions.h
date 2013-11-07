/*
 * uart_functions.h
 *
 *  Created on: Jul 1, 2013
 *      Author: Bono
 */

#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include "stm32f0xx_usart.h"


extern void uart_operation(void);
extern void uart_operation_for_LCD(void);
extern void uart_send_word(uint16_t word_value);
extern void uart_task(void);
#endif /* UART_FUNCTIONS_H_ */
