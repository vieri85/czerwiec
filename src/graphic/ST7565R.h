//*****************************************************************************
//
//! \file ST7565R.h
//! \brief
//! \version V0.0.1
//! \date 3/3/2013
//! \author Shadow(gzbkey)
//! \copy
//!
//! Copyright (c)  2012-2013, Shadow(gzbkey)
//! All rights reserved.
//
//*****************************************************************************

#ifndef _ST7565R_H_
#define _ST7565R_H_
//*****************************************************************************
//
//! \addtogroup CoX_Driver_Lib
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup LCD
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup LCD_Graphic
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup ST7565R_For_ST
//! @{
//
//*****************************************************************************

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_flash.h"
#include "stm32f0xx_spi.h"
#include "stdlib_2.h"

#define MAX_LINE_SENTENCE  6

#define DEBUG_PIN_0 GPIO_Pin_0
#define DEBUG_PIN_1 GPIO_Pin_1
#define DEBUG_PORT GPIOB

typedef enum
{
    cmd,
    dat
}ST7565_cmd_dat_t;


/*menu state type def*/
typedef enum
{
	START_SCR=0,
	CLOCK,
	SET_PARAM,
	PROG_PERFORM,
	STOP_PROGRAM

}main_menu_t;


typedef enum
{
	LINE_1=0,
	LINE_2,
	LINE_3,
	LINE_4
}menu_line_nr_t;


typedef enum
{
	NORMAL,
	VANISH,
	LOWER_LINE,
	FOUR_MARK,
	H_BORER,
	HASH,
	INVERT
}m_type;

typedef struct
{
	/*Pointers to table contain sentences*/
	uint8_t *sentence_in;
	uint8_t	*sentence_med;
	uint8_t *sentence_out;
	/*Line number to operate: 1,2,3 or 4*/
	menu_line_nr_t line_nr;
	/*set position in whole line operation to change by mark*/
	uint8_t mark_symb_nr;
	/*Set type mark - NORMAL is without changes*/
	m_type mark_type;

}line_operation_t;

typedef struct
{
	uint8_t* sentence_ptr;
	uint8_t start_pos;

}WordDescr_t;


typedef struct
{
	WordDescr_t word_descr[MAX_LINE_SENTENCE];
	/*Line number to operate: 1,2,3 or 4*/
	menu_line_nr_t line_nr;
	/*set position in whole line operation to change by mark*/
	uint8_t mark_symb_nr;
	/*Set type mark - NORMAL is without changes*/
	m_type mark_type;
	uint8_t max_ptr_nr;

}LineOperationExtend_t;


typedef struct
{
	m_type type;
	uint8_t symb_nr;

}marker_t;

ST7565_cmd_dat_t ST7565R_CMD, ST7565R_DAT;

line_operation_t line_operation_1;


#define u8 unsigned char
#define u32 uint32_t
//
//don't change it
//
#define SOFTWARE                0
#define HARDWARE                1

//
// use SPI mode ,set HARDWARE or SOFTWARE
//
#define SPI_MODE                HARDWARE

#define SPI_X                   1

//*****************************************************************************
//
//! ST7565R Device config, here must be configured before use the API.
//
//*****************************************************************************
#define ST7565R_RS       GPIO_Pin_1	//Fira_ A0: low - config data, high-graphic data
#define ST7565R_RST      GPIO_Pin_0
#define ST7565R_SID      GPIO_Pin_7
#define ST7565R_SCLK     GPIO_Pin_5

#define ST7565R_PORT     GPIOA

#define LCD_MAX_COLUMN 	128


//*****************************************************************************
//
//! ST7565R macro definition, do not change it
//
//*****************************************************************************
#define ST7565R_RS_H     GPIO_SetBits(ST7565R_PORT,ST7565R_RS)
#define ST7565R_RS_L     GPIO_ResetBits(ST7565R_PORT,ST7565R_RS)
#define ST7565R_RST_H    GPIO_SetBits(ST7565R_PORT,ST7565R_RST)
#define ST7565R_RST_L    GPIO_ResetBits(ST7565R_PORT,ST7565R_RST)
#define ST7565R_SID_H    GPIO_SetBits(ST7565R_PORT,ST7565R_SID)
#define ST7565R_SID_L    GPIO_ResetBits(ST7565R_PORT,ST7565R_SID)
#define ST7565R_SCLK_H   GPIO_SetBits(ST7565R_PORT,ST7565R_SCLK)
#define ST7565R_SCLK_L   GPIO_ResetBits(ST7565R_PORT,ST7565R_SCLK)
#define ST7565R_CS_H     GPIO_SetBits(ST7565R_PORT,ST7565R_CS)
#define ST7565R_CS_L     GPIO_ResetBits(ST7565R_PORT,ST7565R_CS)


//*****************************************************************************
//
//! ST7565R APIs
//
//*****************************************************************************
extern main_menu_t main_menu_state;

extern void ST7565R_Init(void);
extern void ST7565R_Clear_Screen(void);
extern void ST7565R_Display_Num(u8 ucRow, u8 ucColumn, u8 ucNum);
extern void ST7565R_Display_ASCII(u8 ucRow, u8 ucColumn, u8 ucAscii);
extern void ST7565R_Display_CN(u8 ucRow, u8 ucColumn, u8 *pucData);
extern void ST7565R_Display_16x32_Num(u8 ucRow, u8 ucColumn, u8 ucNum);
extern void ST7565R_Display_16x32_Num_Reverse(u8 ucRow, u8 ucColumn, u8 ucNum);
extern void ST7565R_Display_Picture(u8 *pucData);
extern void ST7565R_Write(u8 ucDatOrCmd, u8 ucData);
extern unsigned char zhong[];
extern unsigned char ST7565R_Picture[];
extern void ST7565R_Delay(u32 uTime);
extern void write_symbol(uint8_t type);
extern void lcd_sentence(uint8_t *tab, uint8_t ucRow, u8 symb_nr, m_type mark_type );
extern void menu_header_put(uint8_t *sentence_ptr);
extern void menu_line_put(line_operation_t line_ptr);


extern void menu_line_put_ext(LineOperationExtend_t line_ptr);
//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! @}
//
//*****************************************************************************
#endif /* _ST7565R_H_ */

