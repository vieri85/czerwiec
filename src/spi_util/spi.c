/*
 * spi.c
 *
 *  Created on: Jul 1, 2013
 *      Author: Bono
 */
#include "init.h"
#include "spi.h"
 #include "stdint.h"
uint8_t SPI_TX_free = RESET;
uint8_t spi_data_au[MAX_SPI_BUFFOR];
static uint8_t current_vector = 0;


void SPI1_IRQHandler(void)
{
	//if TX buffer empty
    if(SPI1->SR&BIT01)
    {
    	SPI_TX_free = SET;

    	if(current_vector > 0)
    	{
    		--current_vector;
    		SPI_SendData8(SPI1, spi_data_au[current_vector]);
    	}
    	else
    	{
    		/*disable TX buffer empty interrupt*/
    		SPI1->CR2&=~(BIT07);
    	}

    }
}
uint8_t check_spi_tx_flag(void)
{
	return SPI_TX_free;
}

void reset_spi_tx_flag(void)
{
	SPI_TX_free = RESET;
	SPI1->CR2|=BIT07;
}

/*spi_flush()
 *
 *Is using for update buffer SPI
 */

flush_resp_t spi_flush(uint8_t* data_ptr, uint8_t data_size)
{
	uint8_t a;
	flush_resp_t ret = FLUSH_OK;

	if(current_vector == 0)
	{
			if((MAX_SPI_BUFFOR > data_size)&&(0 != data_size))
			{
				/*Move data to SPI write buffer*/
				for(a=0;a<data_size;a++)
				{
					spi_data_au[a] = *(data_ptr+a);
				}
				current_vector = data_size;

				/*Enable interrupt SPI for empty TX buffer*/
				SPI1->CR2|=BIT07;
			}
			else
			{
				ret = FLUSH_WRONG_SIZE;
			}
	}
	else
	{
		ret = FLUSH_BUSY;
	}

	return ret;

}
