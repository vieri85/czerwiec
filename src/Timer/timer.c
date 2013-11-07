/*
 * timer.c
 *
 *  Created on: Sep 30, 2013
 *      Author: Bono
 */
#include "stm32f0xx_tim.h"
#include "ST7565R.h"

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	if(RESET != TIM_GetITStatus(TIM1, TIM_IT_Update))
	{
		/*Clear interrupt bit*/
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		if(GPIO_ReadInputDataBit(DEBUG_PORT, DEBUG_PIN_1))
			GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);
		else
			GPIO_SetBits(DEBUG_PORT, DEBUG_PIN_1);
	}
}


uint16_t read_encoder(void)
{
	return TIM1->CNT;
}

void reset_encoder(void)
{
	TIM1->CNT = 0;
}
