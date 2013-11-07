/*
 * init.c
 *
 *  Created on: Jun 26, 2013
 *      Author: Bono
 */


#include "init.h"




typedef union
{
	uint16_t cr1_reg;
	struct{
		uint16_t CPHA:1;
		uint16_t CPOL:1;
		uint16_t MSTR:1;
		uint16_t BR:3;
		uint16_t SPE:1;
		uint16_t LSB_FIRST:1;
		uint16_t SSI:1;
		uint16_t SSM:1;
		uint16_t RX_ONLY:1;
		uint16_t CRCL:1;
		uint16_t CRC_NEXT:1;
		uint16_t CRC_EN:1;
		uint16_t BIDI_OE:1;
		uint16_t BIDI_MODE:1;
	}cr1_bits;
}spi_cr1_reg_t;

typedef union
{
	uint16_t cr2_reg;
	struct
	{
		uint16_t RXDMA_EN:1;
		uint16_t TSDMA_EN:1;
		uint16_t SSO_EN:1;
		uint16_t NSSP:1;
		uint16_t FRF:1;
		uint16_t ERRIE:1;
		uint16_t RXNEIE:1;
		uint16_t TXEIE:1;
		uint16_t DS:4;
		uint16_t FRXTH:1;
		uint16_t LDMA_RX:1;
		uint16_t LDMA_TX:1;
		uint16_t Reserwed:1;
	}cr2_bits;

}spi_cr2_reg_t;

static void init_spi(void);
void systick_init(void);
static void debug_pin_init(void);
static void uart_init(void);
static void init_general_gpio_UART(void);
//static void Timer1_init(void);

void init_general_gpio_UART(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void uart_init(void)
{
	  USART_InitTypeDef USART_2_InitStruct;
	  NVIC_InitTypeDef NVIC_InitStructure;

	  USART_2_InitStruct.USART_BaudRate = 4800;
	  USART_2_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	  USART_2_InitStruct.USART_WordLength = USART_WordLength_8b;
	  USART_2_InitStruct.USART_StopBits = USART_StopBits_1;
	  USART_2_InitStruct.USART_Parity = USART_Parity_No ;
	  USART_2_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  init_general_gpio_UART();
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	  GPIO_PinAFConfig(GPIOA, 2, GPIO_AF_1);
	  GPIO_PinAFConfig(GPIOA, 3, GPIO_AF_1);
	  USART_Init(USART2, &USART_2_InitStruct);
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	  USART_Cmd(USART2, ENABLE);
}

void debug_pin_init(void)
{
	  GPIO_InitTypeDef        GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DEBUG_PIN_0 | DEBUG_PIN_1;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(DEBUG_PORT, &GPIO_InitStructure);
	  GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_0);
	  GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);
}

void init_spi(void)
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

	  NVIC_InitTypeDef NVIC_InitStructure;
		spi_cr1_reg_t cr1_conf;
		spi_cr2_reg_t cr2_conf;

		cr1_conf.cr1_bits.BIDI_MODE = 0;
		cr1_conf.cr1_bits.BIDI_OE = 1;
		cr1_conf.cr1_bits.CPHA = 1;
		cr1_conf.cr1_bits.CPOL = 1;
		cr1_conf.cr1_bits.CRCL = 0;
		cr1_conf.cr1_bits.CRC_EN = 0;
		cr1_conf.cr1_bits.CRC_NEXT = 0;
		cr1_conf.cr1_bits.LSB_FIRST =0;
		cr1_conf.cr1_bits.MSTR = 1;
		cr1_conf.cr1_bits.RX_ONLY = 0;
		cr1_conf.cr1_bits.BR = 2;  // prescaler: 32
		cr1_conf.cr1_bits.SPE = 0; //Mozliwe ze trza po inicjalizacji pozostalych bitow ustawic i cr2
		cr1_conf.cr1_bits.SSI = 1;
		cr1_conf.cr1_bits.SSM = 1;

		cr2_conf.cr2_bits.RXDMA_EN = 0;
		cr2_conf.cr2_bits.TSDMA_EN = 0;
		cr2_conf.cr2_bits.SSO_EN = 0;
		cr2_conf.cr2_bits.NSSP = 0;
		cr2_conf.cr2_bits.FRF = 0; //Motorolla mode ???
		cr2_conf.cr2_bits.ERRIE = 0;
		cr2_conf.cr2_bits.RXNEIE = 0; //Do wy³aczenia!!!
		cr2_conf.cr2_bits.TXEIE = 1; // ustaw na 1 gdy interrupt od pustego bufora Tx ma byc
		cr2_conf.cr2_bits.DS = 7; //8bit ramka
		cr2_conf.cr2_bits.FRXTH = 1;
		cr2_conf.cr2_bits.LDMA_RX = 0;
		cr2_conf.cr2_bits.LDMA_TX = 0;


		//There is matter off sequence
		SPI1->CR2 = cr2_conf.cr2_reg;
		SPI1->CR1 = cr1_conf.cr1_reg;
		SPI1->CR1 |= SPI_CR1_SPE;
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);


}

void systick_init(void)
{
	/*SET to 1ms period*/
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick->LOAD  = 16000;      									 /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
    SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */

    systick_timer.timer_1000ms = 0;
    systick_timer.timer_100ms = 0;
    systick_timer.timer_10ms = 0;
}

void DAC_init(void)
{
	/*FIRSTYBLE!!!!!: Enable clock functionality for DAC*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);


	/*Enable module DAC by set bit EN1*/
	DAC->CR |= BIT00;

	/*Trigger by software generating out voltage by set bits TSEL1_0, TSE1_1, TSEL1_2*/
	DAC->CR |= (BIT03|BIT04|BIT05);

	/*Enable Output Buffer to reduce output impedance by set bit BOFF1*/
	DAC->CR |= BIT01;

	/*Set register Data Holding Register to 0,1V voltage value*/
	DAC->DHR8R1 = 9;

	/*Bit SWTRIG1 - Software Trigger is use to trigger update register DAC_DOR1 by DAC_DHR1. After set this bit first APB1 clock
	 * update register and clear flag SWTRIG1
	*/
	DAC->SWTRIGR |= BIT00;



}

#if 0
void Timer1_init(void)
{
	TIM_TimeBaseInitTypeDef timer_init;
	TIM_OCInitTypeDef TIM_OCInitStruct_1;
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);


	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, 6, GPIO_AF_1);

	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 0x00F0;
	timer_init.TIM_Prescaler = 0;
	/*Used only in TIM1*/
	timer_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timer_init);


	/*Configuration of channel 1 for timer 3*/
	TIM_OCInitStruct_1.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct_1.TIM_OutputState = TIM_OutputState_Enable;
	/*Width of pulse in timer count unit*/
	TIM_OCInitStruct_1.TIM_Pulse = 0x00FF;
	/*Polarization output while pulse*/
	TIM_OCInitStruct_1.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct_1);

	/*Set:
	 * Bit 9-8: Clock division to 0
	 * Bit 7  : Disable buffered ARR
	 * Bit6-5 : Edge aligned mode
	 * Bit 4  : upcounter (matter in ceneter alligned or encoder mode)
	 * Bit 3  : counter is not stopped at update event
	 * Bit 1  : Enable update event
	 */
	TIM1->CR1 = 0;
//
//	/*Set:
//	 * Bit2 : Only counter overflow/underflow generate interrupt
//	 *
//	 */
	TIM1->CR1 = BIT02;



	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/*Enable interrupt for timer1, channel:1*/
//	TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);

	TIM_SetAutoreload(TIM3, 0x01FF);

	/*Start timer - Enable it*/
	TIM_Cmd(TIM3, ENABLE);
}
#endif

void init_encoder_tim1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef timer_init;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, 8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, 9, GPIO_AF_2);

	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 0xFFFF;
	timer_init.TIM_Prescaler = 0;
	/*Used only in TIM1*/
	timer_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timer_init);

	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	TIM_SetAutoreload(TIM1, 0xFFFF);
	TIM_Cmd(TIM1, ENABLE);

}


void init_DMA(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}




void init_devices(void)
{
	SystemCoreClockUpdate();
	debug_pin_init();
	systick_init();
	uart_init();
	init_spi();
	DAC_init();
	init_encoder_tim1();
	init_DDS();
}
