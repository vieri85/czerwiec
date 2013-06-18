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

void init_gpio_blink_led(void);

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


//	  GPIOB->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER0_0);
//	  /* Configure PC8 and PC9 in output  mode  */
//
//	  GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1) ;
//	  // Ensure push pull mode selected--default
//
//	  GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1|GPIO_OSPEEDER_OSPEEDR0);
//	  //Ensure maximum speed setting (even though it is unnecessary)
//
//	  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0|GPIO_PUPDR_PUPDR1);
//	  //Ensure all pull up pull down resistors are disabled
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
	  GPIO_PinAFConfig(GPIOA, 2, GPIO_AF_1);
	  GPIO_PinAFConfig(GPIOA, 3, GPIO_AF_1);

	  USART_2_InitStruct.USART_BaudRate = 19200;
	  USART_2_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	  USART_2_InitStruct.USART_WordLength = USART_WordLength_8b;
	  USART_2_InitStruct.USART_StopBits = USART_StopBits_1;
	  USART_2_InitStruct.USART_Parity = USART_Parity_No ;
	  USART_2_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  USART_Init(USART2, &USART_2_InitStruct);

	  init_general_gpio_UART();




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

void systick_init(void)
{
	/*SET to 1ms period*/
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick->LOAD  = 50000;      									 /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
    SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

void init_devices(void)
{
	debug_pin_init();

	//TODO delete after debug (FIRA)
	init_gpio();
	init_gpio_blink_led();
	systick_init();
	usart_init();
}




