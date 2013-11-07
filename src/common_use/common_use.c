/*
 * common_use.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

/* include file */
#include "common_use.h"
#include "stm32f0xx_usart.h"


/*Global variable*/
//static uint8_t RDBI_adres=55;

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


#if 0
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



void RDBI_func(void)
{
	uint8_t send_data=0;
	switch(RDBI_adres)
	{
		case '1':
		{
			send_data = (uint8_t)((SPI1->CR1)>>8);
			break;
		}
		case '2':
		{
			send_data = (uint8_t)((SPI1->CR1));
			break;
		}
		case '3':
		{
			send_data = (uint8_t)((SPI1->CR2)>>8);
			break;
		}
		case '4':
		{
			send_data = (uint8_t)((SPI1->CR2));
			break;
		}
		default:
			send_data = 'A';
			break;


	}


	if(new_receive == 1)
	{
		new_receive = 0;
		USART_SendData(USART2, send_data);
	}
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



//void USART2_IRQHandler_com(void)
//{
//   static uint32_t register_interupt;
//   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//   {
//       letter = USART_ReceiveData(USART2);
//       new_receive = 1;
//       RDBI_adres = letter;
//   }
//   register_interupt = USART2->ISR;
//
//}


void led_blinkin(void)
{
	if(RESET==GPIO_ReadOutputDataBit(GPIOC , GPIO_Pin_8))
		/* Set PC8 and PC9 */
		GPIOC->BSRR = BSRR_VAL;
	else
		GPIOC->BRR = BSRR_VAL;
}


//TBD
void usart_task_operation(void)
{
	uint8_t usrt_send_buffor[10];
	uint8_t current_buffor_byte;

	if(1)
	{
		USART_SendData(USART2, ' ');
	}



}
#endif

void set_debug_pin(uint16_t debug_pin_nr)
{
	GPIO_SetBits(DEBUG_PORT, debug_pin_nr);
}

void reset_debug_pin(uint16_t debug_pin_nr)
{
	  GPIO_ResetBits(DEBUG_PORT, debug_pin_nr);
}
