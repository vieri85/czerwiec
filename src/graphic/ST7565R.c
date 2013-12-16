//*****************************************************************************
//
//! \file ST7565R.c
//! \brief
//! \version V0.0.2
//! \date 2013.3.4
//! \author Shadow(gzbkey)
//! \copy
//!
//! Copyright (c)  2012-2013, Shadow(gzbkey)
//! All rights reserved.
//
//*****************************************************************************

#include "ST7565R.h"
#include "ST7565R_Font.h"
#include "ST7565R_Picture.h"
#include "scheduler.h"
#include "spi.h"
#include <inttypes.h>
#include "menu.h"




/*LCD defines 128x64*/
#define MAX_8x16_LINE_SIGN 16

uint8_t err_sentence[]="err!";
marker_t empty;

uint8_t column_set(uint8_t column);
uint8_t page_adress_set(uint8_t page_nr);
uint8_t check_sentence_size(uint8_t *sentence_ptr);





signed char copy_sentence_to_buf(uint8_t *buffer,uint8_t buf_size,uint8_t start_iter, uint8_t *sentence);

uint8_t page_adress_set(uint8_t page_nr)
{
	if(page_nr<=7)
	{

		ST7565R_Write(ST7565R_CMD, (0xB0+page_nr));
		return 0;
	}

	return 1;


}

uint8_t column_set(uint8_t column)
{

	if(column <= LCD_MAX_COLUMN)
	{


		ST7565R_Write(ST7565R_CMD, ((column/16)|0x10));
		ST7565R_Write(ST7565R_CMD, (column%16));
		return 0;
	}

	return 1;
}

/*
 * Function: lcd_sentence
 *
 * Parameters:
 * tab - pointer to sentence
 * ucRow - line number start from 0
 * symb_nr - start from 0 end on 15, cell number in line to mark
 */
void lcd_sentence(uint8_t *tab, uint8_t ucRow, u8 symb_nr, m_type mark_type )
{

    u8 i, j,k, Column_Temp, Column_L, Column_H;
    u8 ucColumn=0;
    u32 uiHChar;
    u8 mark_local_flg=0;
    static u8 mark_char=0;

    mark_char=0;
    if(mark_type != NORMAL)
    {
    	if((mark_type == VANISH)||(mark_type == INVERT))
    	{
    		mark_local_flg = 1;
    	}
    	else
    	{
    		mark_local_flg = 2;
    		switch(mark_type)
    		{
    		case LOWER_LINE:
    			mark_char =0;
    			break;
    		case HASH:
    			mark_char=16;
    			break;
    		case FOUR_MARK:
    			mark_char=32;
    			break;
    		default:
    			mark_char=0;
    			break;
    		}
    	}
    }

    k=0;
    while(tab[k]!=0)
    {
    	if((tab[k]>31) && (tab[k]<((HGROM_SIZE/16)+32)))
    	{
    		uiHChar = (tab[k] - 32) * 16;
    	}
    	else
    	{
    		uiHChar = 0;
    	}

    	for (i = 0; i < 2; i++)
        {

            //set page address

            ST7565R_Write(ST7565R_CMD, 0xb0 + i + 2 * ucRow);

            Column_Temp = ucColumn * 0x08;
            Column_H = Column_Temp & 0xf0;
            Column_H = Column_H >> 4;
            Column_L = Column_Temp & 0x0f;
            ST7565R_Write(ST7565R_CMD, 0x10 + Column_H);
            ST7565R_Write(ST7565R_CMD, 0x00 + Column_L);
            for (j = 0; j < 8; j++)
            {
            	/*If this nr symbol sentence must be change*/
            	if((symb_nr == k) &&(mark_local_flg != 0))
            	{

            		if(mark_local_flg == 1)
            		{
            			/*For vanish type*/
            			if(mark_type == VANISH)
            			{
            				ST7565R_Write(ST7565R_DAT, 0x00);
            			}
            			/*For invert type*/
            			else
            			{
            				ST7565R_Write(ST7565R_DAT, (~HCGROM[uiHChar]));
            			}


            		}
            		/*All case adds to exist symbol mark*/
            		else if(mark_local_flg == 2)
            		{
            			ST7565R_Write(ST7565R_DAT, (MARK_SIGN[mark_char]|HCGROM[uiHChar]));
            			++mark_char;
            		}


            	}
            	/*No mark changes in this number of sentence symbol*/
            	else
            	{
            		ST7565R_Write(ST7565R_DAT, HCGROM[uiHChar]);
            	}

                uiHChar++;
            }
        }
    	ucColumn=k+1;
    	++k;
    }
}




/*put it to scheduller 100ms*/

void menu_opertion(void)
{
	switch(main_menu_state)
	{
	case START_SCR:
		break;

	case CLOCK:
		break;

	case SET_PARAM:
//		menu_alphabet_500ms();
		break;

	case PROG_PERFORM:
		break;

	case STOP_PROGRAM:

	default:
		break;

	}



}






//
//void screan_creator_128x64(void)
//{
//
//}
////
void menu_header_put(uint8_t *sentence_ptr)
{
	uint8_t lcd_buffer[] = {"                "};
	uint8_t buffer_count;
	uint8_t sentence_count = 0;
	uint8_t sentence_size;

	sentence_size = check_sentence_size(sentence_ptr);

	if(sentence_size <= MAX_8x16_LINE_SIGN)
	{
			buffer_count =((MAX_8x16_LINE_SIGN-sentence_size)/2);
			while(sentence_ptr[sentence_count]!=0)
			{
					lcd_buffer[buffer_count]= sentence_ptr[sentence_count];
					++sentence_count;
					++buffer_count;
			}

		lcd_sentence(lcd_buffer, 0,0, NORMAL);
	}
	else
	{
		lcd_sentence(err_sentence, 0,0,NORMAL);
	}

}

void menu_line_put(line_operation_t line_ptr)
{
	uint8_t  lcd_buffer[] = {"                         "};
	signed char  char_nr;
	uint8_t local;

	local = sizeof(lcd_buffer);
	char_nr = copy_sentence_to_buf(lcd_buffer, sizeof(lcd_buffer), 0, line_ptr.sentence_in);
	if(-1 != char_nr)
	{
		local = (sizeof(lcd_buffer)-(uint8_t)char_nr);
		char_nr = copy_sentence_to_buf(lcd_buffer,sizeof(lcd_buffer),(uint8_t)char_nr, line_ptr.sentence_med);
		if(-1 != char_nr)
		{
			local = (sizeof(lcd_buffer)-(uint8_t)char_nr);
			char_nr = copy_sentence_to_buf(lcd_buffer, sizeof(lcd_buffer), (uint8_t)char_nr, line_ptr.sentence_out);
			if(-1 != char_nr)
			{
				lcd_sentence(lcd_buffer, line_ptr.line_nr, line_ptr.mark_symb_nr ,line_ptr.mark_type);
				return;
			}
		}
	}

	lcd_sentence(err_sentence, line_ptr.line_nr, 0 ,NORMAL);

}

void menu_line_put_ext(LineOperationExtend_t line_ptr)
{
	uint8_t  lcd_buffer[] = {"                "};
	uint8_t word_nr=0;

	for(;word_nr <= line_ptr.max_ptr_nr;word_nr++)
	{

		if(line_ptr.word_descr[word_nr].sentence_ptr != NULL)
		{

			copy_sentence_to_buf(lcd_buffer,16,line_ptr.word_descr[word_nr].start_pos,
				 	 	 	 	 line_ptr.word_descr[word_nr].sentence_ptr);
		}

	}

	lcd_sentence(lcd_buffer, line_ptr.line_nr, line_ptr.mark_symb_nr ,line_ptr.mark_type);

}



uint8_t check_sentence_size(uint8_t *sentence_ptr)
{
	uint8_t k=0;

	while(sentence_ptr[k]!=0)
	{
		++k;
		if(k>MAX_8x16_LINE_SIGN)
		{
			break;
		}
	}
	return k;
}

/*Funct: copy_sentence_to_buf()
 *
 * This funct copy one sentence (table of char ended NULL)
 * into buffer(table of char ended NULL).
 * Func return value >0 if all sentence fir into buffer,
 * otherwise if buffer will overflow then return -1.
 */

signed char copy_sentence_to_buf(uint8_t *buffer,uint8_t buf_size,uint8_t start_iter, uint8_t *sentence)
{
	uint8_t k=0;

	while(start_iter<127)
	{
		if(sentence[k]==0)
		{
			break;
		}
		else if((buffer[start_iter]==0) || (start_iter >= buf_size))
		{
			return -1;
			break;
		}
		buffer[start_iter] = sentence[k];
		++k;
		++start_iter;
	}
	return start_iter;
}


/***********************************************************************************************************************
 **********************************************  OLD FUNCTIONS	********************************************************
 ***********************************************************************************************************************/


void ST7565R_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure SPI1 pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = ST7565R_SCLK | ST7565R_SID;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = ST7565R_RS | ST7565R_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);

}

//*****************************************************************************
//
//! \brief Write one byte (SPI mode)
//!
//! \param ucByte is data(8bit).
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_SPI_Writebyte(unsigned char ucByte)
{
		spi_flush(&ucByte, 1);
}

//*****************************************************************************
//
//! \brief Delay ms
//!
//! \param usTime is delay value.
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Delay(u32 uTime)
{
    u32 i;

    for (i = 0; i < uTime; i++)
    	;
}

//*****************************************************************************
//
//! \brief Send data or command to ST7565R
//!
//! \param ucDatOrCmd decide send data or command.
//! \param ucSendByte is data(8bit) or command(8bit).
//!
//! ucDatOrCmd can be ST7920_DAT or ST7920_CMD
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Write(u8 ucDatOrCmd, u8 ucData)
{

    if (ucDatOrCmd == ST7565R_DAT) /* DAT */
    {
        ST7565R_RS_H;
    }
    else /* CMD */
    {
        ST7565R_RS_L;
    }
    ST7565R_SPI_Writebyte(ucData);

}

//*****************************************************************************
//
//! \brief Initialization ST7565R
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Init(void)
{


    ST7565R_DAT = dat;
    ST7565R_CMD = cmd;
    //GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);
    ST7565R_RS_L;
    ST7565R_RST_L;
    ST7565R_SID_L;
    ST7565R_SCLK_L;
    //GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);

	systic_delay_ms(500);
	//GPIO_SetBits(DEBUG_PORT, DEBUG_PIN_1);


    ST7565R_GPIO_Init();
    ST7565R_RST_L;
    systic_delay_ms(200);
    ST7565R_RST_H;


    ST7565R_Write(ST7565R_CMD, 163); /*LCD bias set: 1/7 */
    ST7565R_Write(ST7565R_CMD, 161); /*ADC select: reverse */
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 192); /*Common output mode select: normal dir. */
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 47); /*Power control set to 47*/
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 0x81); /*Electronic volume mode set*/
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 30); /*Electronic volume register set to 30*/
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 36); /*Vo voltage regulator internal resistor set to 36*/
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 0x40); /*Display start line set to 0*/
    systic_delay_ms(30);
    ST7565R_Write(ST7565R_CMD, 0xaf); /*LCD ON*/
    ST7565R_Clear_Screen();

    init_for_menu();
}

//*****************************************************************************
//
//! \brief Clear the srceen of ST7565R
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Clear_Screen(void)
{
    u8 i, j;

    for (i = 0; i < 8; i++)
    {

        ST7565R_Write(ST7565R_CMD, 0xb0 + i);
        ST7565R_Write(ST7565R_CMD, 0x10);
        ST7565R_Write(ST7565R_CMD, 0x00);
        for (j = 0; j < 132; j++)
        {
            ST7565R_Write(ST7565R_DAT, 0x00);
        }
    }
}



#if(0)

//*****************************************************************************
//
//! \brief Display a number (16*32)
//!
//! \param ucRow is row
//! \param ucColumn is column
//! \param ucNum is a number for display
//!
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Display_16x32_Num(u8 ucRow, u8 ucColumn, u8 ucNum)
{
    u8 i, j, Column_Temp, Column_L, Column_H;

    ucNum = ucNum * 60;
    for (i = 0; i < 4; i++)
    {

        //
        //set page address,设置行地址
        //
        ST7565R_Write(ST7565R_CMD, 0xb0 + i + 2 * ucRow);
        //
        //设置列高低地址
        //
        Column_Temp = ucColumn * 16;
        Column_H = Column_Temp & 0xf0;
        Column_H = Column_H >> 4;
        Column_L = Column_Temp & 0x0f;
        ST7565R_Write(ST7565R_CMD, 0x10 + Column_H);
        ST7565R_Write(ST7565R_CMD, 0x00 + Column_L);
        for (j = 0; j < 15; j++)
        {
            ST7565R_Write(ST7565R_DAT, Num_16x32[ucNum]);
            ucNum++;
        }
    }
}

//*****************************************************************************
//
//! \brief Reverse display a number (16*32)
//!
//! \param ucRow is row
//! \param ucColumn is column
//! \param ucNum is a number for display
//!
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Display_16x32_Num_Reverse(u8 ucRow, u8 ucColumn, u8 ucNum)
{
    u8 i, j, Column_Temp, Column_L, Column_H;
    u32 uiNum_Temp;

    uiNum_Temp = ucNum * 60;
    for (i = 0; i < 4; i++)
    {

        //
        //set page address,设置行地址
        //
        ST7565R_Write(ST7565R_CMD, 0xb0 + i + 2 * ucRow);
        //
        //设置列高低地址
        //
        Column_Temp = ucColumn * 16;
        Column_H = Column_Temp & 0xf0;
        Column_H = Column_H >> 4;
        Column_L = Column_Temp & 0x0f;
        ST7565R_Write(ST7565R_CMD, 0x10 + Column_H);
        ST7565R_Write(ST7565R_CMD, 0x00 + Column_L);
        for (j = 0; j < 15; j++)
        {
            ST7565R_Write(ST7565R_DAT, ~Num_16x32[uiNum_Temp]);
            uiNum_Temp++;
        }
    }
}


//*****************************************************************************
//
//! \brief Display a picture (12832)
//!
//! \param *pucData is array of picture data
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Display_Picture(u8 *pucData)
{
    u8 i, j;

    for (i = 0; i < 4; i++)
    {

        //
        //set page address,设置行地址
        //
        ST7565R_Write(ST7565R_CMD, 0xb0 + i);
        //
        //设置列高低地址
        //
        ST7565R_Write(ST7565R_CMD, 0x10);
        ST7565R_Write(ST7565R_CMD, 0x00);
        for (j = 0; j < 128; j++)
        {
            ST7565R_Write(ST7565R_DAT, *pucData);
            pucData++;
        }
    }
}


void write_symbol(uint8_t type)
{
	uint8_t kol=0;
	uint8_t pagge=0;

	if(type>0)
	{
			for(pagge=0;pagge<8;pagge++)
	{
		column_set(0);
		page_adress_set(pagge);
		for(kol=0;kol<128;kol++)
		{
			ST7565R_Write(ST7565R_DAT,255 );
		}
	}
	}
	else
	{
		for(pagge=0;pagge<8;pagge++)
	{
		column_set(0);
		page_adress_set(pagge);
		for(kol=0;kol<128;kol++)
		{
			ST7565R_Write(ST7565R_DAT, 0);
		}
	}
	}




}
#endif

