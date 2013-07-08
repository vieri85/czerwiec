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
#include "stm32f0xx_spi.h"
#include "spi.h"

#define DEBUG_PIN_0 GPIO_Pin_0
#define DEBUG_PIN_1 GPIO_Pin_1
#define DEBUG_PORT GPIOB

//*****************************************************************************
//
//! \brief Initialization GPIO
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure SPI1 pins: SCK, MISO and MOSI -------------------------------*/
    GPIO_InitStructure.GPIO_Pin = ST7565R_SCLK | ST7565R_SID;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = ST7565R_RS | ST7565R_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);

}
#if SPI_MODE==HARDWARE
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

#endif

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
    u32 i, j;

    for (i = 0; i < uTime; i++)
        for (j = 0; j < 1040; j++)
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
#if SPI_MODE==SOFTWARE
    u8 i, uTime;

    if (ucDatOrCmd == ST7565R_DAT) /* DAT */
    {
        ST7565R_RS_H;
    }
    else /* CMD */
    {
        ST7565R_RS_L;
    }

    for (i = 0; i < 8; i++)
    {
        if (ucData & 0x80)
        {
            ST7565R_SID_H;
        }
        else
        {
            ST7565R_SID_L;
        }
        ST7565R_SCLK_H;
        while (uTime--)
            ;
        ST7565R_SCLK_L;
        while (uTime--)
            ;
        ucData <<= 1;
    }
#else  //HARDWARE
    if (ucDatOrCmd == ST7565R_DAT) /* DAT */
    {
        ST7565R_RS_H;
    }
    else /* CMD */
    {
        ST7565R_RS_L;
    }
    ST7565R_SPI_Writebyte(ucData);
#endif
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
    ST7565R_RS_L;
    ST7565R_RST_L;
    ST7565R_SID_L;
    ST7565R_SCLK_L;
    GPIO_ResetBits(DEBUG_PORT, DEBUG_PIN_1);

    ST7565R_Delay(500);
	 GPIO_SetBits(DEBUG_PORT, DEBUG_PIN_1);


    ST7565R_GPIO_Init();
    ST7565R_RST_L;
    ST7565R_Delay(200);
    ST7565R_RST_H;


    ST7565R_Write(ST7565R_CMD, 163); /*LCD bias set: 1/7 */
    ST7565R_Write(ST7565R_CMD, 161); /*ADC select: reverse */
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 192); /*Common output mode select: normal dir. */
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 47); /*Power control set to 47*/
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 0x81); /*Electronic volume mode set*/
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 30); /*Electronic volume register set to 30*/
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 36); /*Vo voltage regulator internal resistor set to 36*/
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 0x40); /*Display start line set to 0*/
    ST7565R_Delay(5);
    ST7565R_Write(ST7565R_CMD, 0xaf); /*LCD ON*/
    ST7565R_Clear_Screen();
    ST7565R_Display_ASCII(10, 64, 65); //LCD "A"
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

//*****************************************************************************
//
//! \brief Display a number within HCGROM
//!
//! \param ucRow is row
//! \param ucColumn is column
//! \param ucNum is number (0-9)
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Display_Num(u8 ucRow, u8 ucColumn, u8 ucNum)
{
    u8 i, j, Column_Temp, Column_L, Column_H;

    ucNum = ucNum * 16;
    for (i = 0; i < 2; i++)
    {

        //
        //set page address,设置行地址
        //
        ST7565R_Write(ST7565R_CMD, 0xb0 + i + 2 * ucRow);
        //
        //设置列高低地址
        //
        Column_Temp = ucColumn * 0x08;
        Column_H = Column_Temp & 0xf0;
        Column_H = Column_H >> 4;
        Column_L = Column_Temp & 0x0f;
        ST7565R_Write(ST7565R_CMD, 0x10 + Column_H);
        ST7565R_Write(ST7565R_CMD, 0x00 + Column_L);
        for (j = 0; j < 8; j++)
        {
            ST7565R_Write(ST7565R_DAT, HCGROM[ucNum]);
            ucNum++;
        }
    }
}

//*****************************************************************************
//
//! \brief Display a Ascii within HCGROM
//!
//! \param ucRow is row
//! \param ucColumn is column
//! \param ucAscii is Ascii
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Display_ASCII(u8 ucRow, u8 ucColumn, u8 ucAscii)
{
    u8 i, j, Column_Temp, Column_L, Column_H;
    u32 uiHChar;

    //
    //ascii转该字的数组起始位
    //
    uiHChar = (ucAscii - 23) * 16;

    for (i = 0; i < 2; i++)
    {

        //
        //set page address,设置行地址
        //
        ST7565R_Write(ST7565R_CMD, 0xb0 + i + 2 * ucRow);
        //
        //设置列高低地址
        //
        Column_Temp = ucColumn * 0x08;
        Column_H = Column_Temp & 0xf0;
        Column_H = Column_H >> 4;
        Column_L = Column_Temp & 0x0f;
        ST7565R_Write(ST7565R_CMD, 0x10 + Column_H);
        ST7565R_Write(ST7565R_CMD, 0x00 + Column_L);
        for (j = 0; j < 8; j++)
        {
            ST7565R_Write(ST7565R_DAT, HCGROM[uiHChar]);
            uiHChar++;
        }
    }
}

//*****************************************************************************
//
//! \brief Display the Chinese
//!
//! \param ucRow is row
//! \param ucColumn is column
//! \param *pucData is string
//!
//! \return None.
//
//*****************************************************************************
void ST7565R_Display_CN(u8 ucRow, u8 ucColumn, u8 *pucData)
{
    u8 i, j, Column_Temp, Column_L, Column_H;

    for (i = 0; i < 2; i++)
    {

        //
        //set page address,设置行地址
        //
        ST7565R_Write(ST7565R_CMD, 0xb0 + i + 2 * ucRow);
        //
        //设置列高低地址
        //
        Column_Temp = ucColumn * 0x10;
        Column_H = Column_Temp & 0xf0;
        Column_H = Column_H >> 4;
        Column_L = Column_Temp & 0x0f;
        ST7565R_Write(ST7565R_CMD, 0x10 + Column_H);
        ST7565R_Write(ST7565R_CMD, 0x00 + Column_L);
        for (j = 0; j < 16; j++)
        {
            ST7565R_Write(ST7565R_DAT, *pucData);
            pucData++;
        }
    }
}


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











