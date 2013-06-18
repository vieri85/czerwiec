/*
 * common_use.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

/* include file */
#include "common_use.h"


/*Global variable*/
uint8_t new_receive = 0;
static uint8_t letter=255;

uint8_t BCD_TO_DEC(uint8_t* bufor,uint8_t *dec)
{
	uint8_t ret=1;
	uint16_t temp=0;
		temp=bufor[0] * 100;
		temp+=bufor[1]*10;
		temp+=bufor[2];
		if(temp<256)
		{
			*dec = (uint8_t)temp;
			ret=0;
		}
	return ret;
}



void uart_interface(void)
{

	static uint8_t k=3;
	static uint8_t bufor[3];
	uint8_t i;
	uint8_t dec_number=0;
	uint8_t st_send_type= cmd;
	USART_SendData(USART2, dec_number);
	if(0 != new_receive)
	{
		 if(letter == 0x0D)
		 {
			 for(i=0;i<3;i++)
			 {
				  bufor[i]=0;
			 }
			 k=0;
			 USART_SendData(USART2, 0x0D);
			 if(st_send_type == cmd)
			 {
				 USART_SendData(USART2, 0x63); //send "c"
			 }
			 else
			 {
				 USART_SendData(USART2, 0x64); //send "d"
			 }

		 }

		 if(letter == 0x64)
		 {
		  st_send_type = dat;
		 }
		 else if(letter == 0x63)
		 {
		  st_send_type = cmd;
		 }


		 if((letter>0x29) && (letter <0x3A))
		 {
			 if(k<3)
			 {

				 bufor[k] = letter-48;
				 ++k;
				 USART_SendData(USART2, letter);
			 }

			 if(k>=3)
			 {

				 if(0 == BCD_TO_DEC(bufor,&dec_number))
				 {
					 USART_SendData(USART2, 0x74);	//send "t"
					 ST7565R_Delay(10);
					 USART_SendData(USART2, dec_number);
					 ST7565R_Write(st_send_type, dec_number);
				 }
				 else
				 {
					 USART_SendData(USART2, 0x6e);  //send "n"
					 ST7565R_Delay(10);
				 }
				 USART_SendData(USART2, 0x0D); //send "enter" character
				 ST7565R_Delay(10);
				 if(st_send_type == cmd)
				 {
					 USART_SendData(USART2, 0x63); //send "c"
				 }
				 else
				 {
					 USART_SendData(USART2, 0x64); //send "d"
				 }
				 k=0;
			 }
		 }

		 new_receive = 0;

	}
}

void delay (int a)
{
	volatile int i,j;

	for (i=0 ; i < a ; i++)
	{
		//uart_interface();
		j++;
	}

	return;
}



uint8_t letter_check(void)
{
	return letter;
}

uint8_t new_receive_check(void)
{
	return new_receive;
}

void new_receive_set(uint8_t value)
{
	new_receive = value;
}

void set_debug_pin(uint16_t debug_pin_nr)
{
	GPIO_SetBits(DEBUG_PORT, debug_pin_nr);
}

void reset_debug_pin(uint16_t debug_pin_nr)
{
	  GPIO_ResetBits(DEBUG_PORT, debug_pin_nr);
}

void USART2_IRQHandler(void)
{
   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
   {
       letter = USART_ReceiveData(USART2);
       new_receive = 1;
   }


   // if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
   // {
   //    USART_SendData(USART1, T_Buff[T_Index++]);
   //    if(T_Buff[T_Index-1] == 0x0d || T_Index == BUFF_SIZE)
   //    {
   //       USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
   //       T_Index = 0;
   //    }
   // }
}
