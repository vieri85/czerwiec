/*
 * common_use.c
 *
 *  Created on: 14-06-2013
 *      Author: Bono
 */

#include "common_use.h"


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

void delay (int a)
{
	volatile int i,j;

	for (i=0 ; i < a ; i++)
	{
		j++;
	}

	return;
}
