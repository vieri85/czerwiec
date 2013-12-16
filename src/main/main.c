/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include "task_functions.h"
#include "ST7565R.h"
#include "uart_functions.h"
#include "spi.h"
#include "syscalls.h"
#include "init.h"
/* Private typedef */

/* Private define  */


/* Private variables */


/* Private function prototypes */


/* Private functions */

/* Global variables */
uint32_t timer=0;
uint8_t  timerFlag=0;
uint8_t SPI_TX_free;
uint8_t Null_tab[]=" ";
uint8_t napis_4[]="Cmok:";
uint8_t napis_5[]="Hz";
uint8_t ii;
uint8_t uu;


inline void init_gpio(void)
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




int main(void)
{



  ii = 0;

  init_gpio();
  SystemCoreClockUpdate();

  //INIT SPI AND OTHERS
 init_devices();
 ST7565R_Init();

  while (1)
  {

	  if (check_timerFlag())
	  {
		  reset_timerFlag();
		  ii++;

		  /* Toggle LED1 */
//		  if (ii == 1)
//		  {
//
//			  LED_PORT->BSRR = LED1;
//
//		  }
//		  else if (ii == 2)
//		  {
//			  ii = 0;
//			  LED_PORT->BRR = LED1;
//			  //menu_header_put(napis_3);
//
//		  }
	  }
      if(GPIO_ReadInputDataBit(KEY_PORT, KEY))
	  {
		  /* USER key pressed */
		  if (ii == 1)
		  {
			  GPIO_SetBits(LED_PORT, LED2);
		  }
	  }
	  else
	  {
		  GPIO_ResetBits(LED_PORT, LED2);
	  }
  }
  return 0;
}

