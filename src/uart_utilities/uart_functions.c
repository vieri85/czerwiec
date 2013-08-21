/*
 * uart_functions.c
 *
 *  Created on: Jul 1, 2013
 *      Author: Bono
 */

#include "uart_functions.h"
#include <stdint.h>
#include "inttypes.h"
#include "ST7565R.h"
#include "keyboard.h"
#include "common_use.h"


uint8_t letter=255;
uint8_t new_receive = 0;




/*OPIS uart_operation()
 * Uzywamy do komunikacji Terminal v1.9b , wlaczamy tryb "string"
 * Jesli dziala poprawnie to naciskanie "Enter" powoduje wyswietlenie "d" lub "c" w górnym oknie (odebrane)
 * informuj¹c ze jestesmy w trybie wysylanai danych lub command.
 * Jesli tak jest to mozemy wpisac 3 cyfry i jesli sa poprawnie zweryfikowane
 * to pokaze sie literka "t" za nimi jesli nie to "n".
 * Lista komend jest w pliku ST7565r_COMMANDS.xls
 *
 */


void uart_operation(void)
{
	static uint8_t i, bufor[5],k,dec_number;
	static ST7565_cmd_dat_t st_send_type=cmd;
	  if(0 != new_receive)
	  {
		  if(letter == 0x0D)
		  {
			  for(i=0;i<3;i++)
			  {
				  bufor[i]=0;
			  }
			  k=0;
			  //0x0D cariage return
			  USART_SendData(USART2, 0x0D);
			  if(st_send_type == cmd)
			  {
			 	 USART_SendData(USART2, 'c'); //send "c"
			  }
			  else
			  {
			 	 USART_SendData(USART2, 'd'); //send "d"
			  }

		  }

		  if(letter == 'd')
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



void uart_operation_for_LCD(void)
{
	static uint8_t repeat_ind=0;
	  if((0 != new_receive))// &&(repeat_ind!=letter))
	  {
		  if(letter == '-')
		  {
			  Button_Set(LEFT);
			  USART_SendData(USART2, '-');
		  }
		  else if((letter == '+'))
		  {
			  Button_Set(RIGHT);
			  USART_SendData(USART2, '+');
		  }
		  else if((letter == ','))
		  {
			  USART_SendData(USART2, ',');
		  }
		  else if((letter == '.'))
		  {
			  USART_SendData(USART2, '.');
		  }
		  else if((letter == '*'))
		  {
			  Button_Set(ENTER);
			  USART_SendData(USART2, '*');
		  }
		  else if(letter == '/')
		  {
			  Button_Set(ESC);
			  USART_SendData(USART2, '/');
		  }

		  new_receive = 0;

	  }

	  repeat_ind = letter;
	  letter=0;

}

void USART2_IRQHandler(void)
{
	//USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
   {
       letter = USART_ReceiveData(USART2);
       new_receive = 1;
   }
   //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}




