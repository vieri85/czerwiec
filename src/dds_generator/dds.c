/*
 * dds.c
 *
 *  Created on: Oct 24, 2013
 *      Author: Bono
 */
#include "dds.h"
//volatile uint32_t delay_lopp;
void delay_us(uint16_t delay_us)
{
	volatile uint16_t del=delay_us;
	//delay_lopp = (delay_us * SystemCoreClock)/16000000UL;
	while(del>0)
	{
		del--;
	}
}

void set_frequency(uint32_t frequency_32)
{
	uint32_t data_freq = (frequency_32*DDS_FACTOR)/DDS_QUARC_VALUE;
	uint8_t bit_index = 0;

	for(;bit_index<40;bit_index++)
	{
		GPIO_WriteBit(GPIOC, DDS_SERIAL_DATA, (data_freq&0x00000001U));
		GPIO_SetBits(GPIOC, DDS_W_CLK);
		GPIO_ResetBits(GPIOC, DDS_W_CLK);
		data_freq = data_freq>>1;
	}
	GPIO_SetBits(GPIOC, DDS_FQ_UD);
	delay_us(5);
	GPIO_ResetBits(GPIOC, DDS_FQ_UD);
}

void init_DDS(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DDS_W_CLK|DDS_FQ_UD|DDS_SERIAL_DATA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*Set to low clock and data load pin of DDS*/
	GPIO_ResetBits(GPIOC, DDS_W_CLK);
	GPIO_ResetBits(GPIOC, DDS_FQ_UD);

	/*Enable serial mode by write parallel value of data*/
    GPIO_SetBits(GPIOC, DDS_W_CLK);
	GPIO_ResetBits(GPIOC, DDS_W_CLK);
	delay_us(5);
	GPIO_SetBits(GPIOC, DDS_FQ_UD);
	delay_us(5);
	GPIO_ResetBits(GPIOC, DDS_FQ_UD);

	/*Set 0 [Hz] frequency after start system*/
	set_frequency(0);
}
