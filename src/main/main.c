/******************************************************************************
  * @file    IO_Toggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
  ******************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "inttypes.h"
#include "stm32f0xx.h"
#include "init.h"
#include "common_use.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BSRR_VAL        0x0300


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

	init_devices();

	while (1)
	{
		/* Set PC8 and PC9 */
		GPIOC->BSRR = BSRR_VAL;
		reset_debug_pin(DEBUG_PIN_1);
		delay(500000);
		set_debug_pin(DEBUG_PIN_1);
		/* Reset PC8 and PC9 */
		GPIOC->BRR = BSRR_VAL;
		delay(500000);
	}
	
	return 0;
}



