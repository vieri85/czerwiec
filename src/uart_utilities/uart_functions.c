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

#define BUFF_SIZE	20U

uint8_t BCD_TO_DEC2(uint8_t* bufor,u8 *dec);
uint8_t* uart_free_buf_size(void);
uint8_t uart_write_buf(uint8_t* data_ptr, uint8_t size);

uint8_t letter=255;
uint8_t new_receive = 0;

uint8_t uart_buffor[BUFF_SIZE];
uint8_t buf_ind_to_write = 0;
uint8_t buf_ind_to_send = 0;
uint8_t buf_free_size = BUFF_SIZE;

uint8_t send_buf[]="      ";


/*OPIS uart_operation()
 * Uzywamy do komunikacji Terminal v1.9b , wlaczamy tryb "string"
 * Jesli dziala poprawnie to naciskanie "Enter" powoduje wyswietlenie "d" lub "c" w górnym oknie (odebrane)
 * informuj¹c ze jestesmy w trybie wysylanai danych lub command.
 * Jesli tak jest to mozemy wpisac 3 cyfry i jesli sa poprawnie zweryfikowane
 * to pokaze sie literka "t" za nimi jesli nie to "n".
 * Lista komend jest w pliku ST7565r_COMMANDS.xls
 *
 */

#if 0
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

				 if(0 == BCD_TO_DEC2(bufor,&dec_number))
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
#endif
void uart_send_word(uint16_t word_value)
{
	static uint16_t hist_value = 0;
	uint8_t index = 5;


	if(hist_value != word_value)
	{
		hist_value = word_value;
		while(index>0)
		{
			--index;
			send_buf[index] = ((word_value%10) + 0x30);
			word_value/=10;
		}

		/*after set all ascii sign represent word_value will be set enter char in sedn buffer*/
		send_buf[5] = 0x0D;

		(void)uart_write_buf(send_buf, 6);
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

uint8_t BCD_TO_DEC2(uint8_t* bufor,u8 *dec)
{
	u8 ret=1;
	uint16_t temp=0;
		temp=bufor[0] * 100;
		temp+=bufor[1]*10;
		temp+=bufor[2];
		if(temp<256)
		{
			*dec = (u8)temp;
			ret=0;
		}
	return ret;
}



void uart_task(void)
{

	if(buf_free_size < BUFF_SIZE)
	{
		USART_SendData(USART2, uart_buffor[buf_ind_to_send]);
		/*UWAGA MOZLIWY BUG!!!!*/
		if((BUFF_SIZE-1) <= buf_ind_to_send)
		{
			buf_ind_to_send = 0;
		}
		else
		{
			++buf_ind_to_send;
		}

		++buf_free_size;
	}
	else
	{
		/*Nothing to send*/
	}

}

uint8_t* uart_free_buf_size(void)
{
	return &buf_free_size;
}


uint8_t uart_write_buf(uint8_t* data_ptr, uint8_t size)
{
	uint8_t ret = 0;
	uint8_t *free_size_ptr = uart_free_buf_size();
	uint8_t indx = 0;

	if(free_size_ptr[0] >= size)
	{
		while( indx < size )
		{
			uart_buffor[buf_ind_to_write] = data_ptr[indx];
			++buf_ind_to_write;
			if( ((buf_ind_to_write) >= BUFF_SIZE) )
			{
				buf_ind_to_write = 0;
			}
			++indx;
		}

		free_size_ptr[0]-=size;
	}
	else
	{
		ret = 1;
	}
	/*enable uart interrupt here!!!!*/
	return ret;
}
























