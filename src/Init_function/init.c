/*
 * init.c
 *
 *  Created on: 23-05-2013
 *      Author: Bono
 */
#include "init.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "scheduler.h"

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

void init_gpio_blink_led(void);
void spi_init(void);
void init_general_gpio_UART(void);

inline void init_general_gpio_UART(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void debug_pin_init(void)
{

	  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	  GPIOB->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER0_0);
	  /* Configure PC8 and PC9 in output  mode  */

	  GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1) ;
	  // Ensure push pull mode selected--default

	  GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1|GPIO_OSPEEDER_OSPEEDR0);
	  //Ensure maximum speed setting (even though it is unnecessary)

	  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0|GPIO_PUPDR_PUPDR1);
	  //Ensure all pull up pull down resistors are disabled

	  GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_0);
	  GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);
}

void usart_init(void)
{
	  USART_InitTypeDef USART_2_InitStruct;
	  NVIC_InitTypeDef NVIC_InitStructure;



	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);


	  USART_2_InitStruct.USART_BaudRate = 4800;
	  USART_2_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	  USART_2_InitStruct.USART_WordLength = USART_WordLength_8b;
	  USART_2_InitStruct.USART_StopBits = USART_StopBits_1;
	  USART_2_InitStruct.USART_Parity = USART_Parity_No ;
	  USART_2_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  USART_Init(USART2, &USART_2_InitStruct);

	  init_general_gpio_UART();

	  GPIO_PinAFConfig(GPIOA, 2, GPIO_AF_1);
	  GPIO_PinAFConfig(GPIOA, 3, GPIO_AF_1);

	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	  USART_Cmd(USART2, ENABLE);
}


void init_gpio(void)
{
	  GPIO_InitTypeDef        GPIO_InitStructure;
	  /* GPIOA-C Periph clock enable */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	  /* Configure
	   * PA0 USER Button input
	   * */
	  GPIO_InitStructure.GPIO_Pin = KEY;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(KEY_PORT, &GPIO_InitStructure);

	  /* Configure PC8 and PC9 in output pushpull mode
	   * PC8 = LD3 Green LED
	   * PC9 = LD4 Blue LED
	   * */
	  GPIO_InitStructure.GPIO_Pin = LED1 | LED2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(LED_PORT, &GPIO_InitStructure);

	  GPIO_SetBits(LED_PORT, LED1);
	  GPIO_ResetBits(LED_PORT, LED1);
	  LED_PORT->BSRR = LED2;
	  LED_PORT->BRR = LED2;


}




void init_gpio_blink_led(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0|GPIO_MODER_MODER0_0) ;
	/* Configure PC8 and PC9 in output  mode  */

	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_0) ;
	// Ensure push pull mode selected--default

	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9);
	//Ensure maximum speed setting (even though it is unnecessary)

	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9);
	//Ensure all pull up pull down resistors are disabled
}



void spi_init(void)
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


void st7565r_init_pins(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = ST7565R_RS | ST7565R_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);
}

void init_devices(void)
{
	debug_pin_init();
	init_gpio();
	init_gpio_blink_led();
	systick_init();
	usart_init();

	spi_init();
	//st7565r_init_pins();
	ST7565R_Init();

}




