/*
 * task_functions.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#include "task_functions.h"
#include "stm32f0xx_usart.h"

void task_every_1ms(void)
{

}

void task_every_10ms(void)
{
	RDBI_func();
}

void task_every_100ms(void)
{
	led_blinkin();
}

void task_every_1000ms(void)
{
	static uint8_t dec_number=0;
	static uint16_t cr1;
	static uint16_t cr2;
	static uint16_t sr;
	static uint16_t check;
	static uint16_t uart_conf;
	dec_number++;
//	while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) == RESET );
//	ST7565R_Write(dat, dec_number);

check =889;
	cr1 = SPI1->CR1;
	cr1 = SPI1->CR2;
	sr = SPI1->SR;
	uart_conf = USART2->CR1;

	//spi send data
	if(1 == (SPI1->SR & BIT01))
	{
		SPI1->DR = 128;
	}




}
