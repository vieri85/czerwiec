/*
 * spi.h
 *
 *  Created on: Jul 1, 2013
 *      Author: Bono
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"

typedef enum
{
	FLUSH_OK=0,
	FLUSH_BUSY,
	FLUSH_WRONG_SIZE,
}flush_resp_t;

#define MAX_SPI_BUFFOR	25


extern uint8_t check_spi_tx_flag(void);
extern void reset_spi_tx_flag(void);
extern flush_resp_t spi_flush(uint8_t* data_ptr, uint8_t data_size);
#endif /* SPI_H_ */
