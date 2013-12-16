/*
 * menu.h
 *
 *  Created on: Jul 23, 2013
 *      Author: Bono
 */

#ifndef MENU_H_
#define MENU_H_


#include "keyboard.h"
#include <inttypes.h>
#include "ST7565R.h"
#include "timer.h"

#define MAX_PROG_NAME_SIGN		10U
#define PROG_NUM_ELEMNT_NUM     11U
#define MARK_NAME_OFFSET 		6U
#define PROG_NAME_LENGTH 		10U
#define LINE_LENGTH 			15U
#define ALPHABET_LENGTH 		42U
#define ALPHA_POS_OFFSET		8U
#define MAX_FREQ_TIME_ON_PROG	15U
#define PROGRAM_NUMBER			10U
#define OPTION_NUMBER			6U

/*Frequency and time selector*/
#define LP_NR				0U
#define ARROW_SELECT_FREQ	1U
#define FREQ_VALUE			2U
#define ARROW_SELECT_TIME	3U
#define TIME_VALUE			4U
#define PROGRES_SIGN		5U

#define LP_START_POSITION    1U
#define FREQ_START_POSITION 3U
#define FREQ_END_POSITION   8U
#define TIME_ONLY_POSITION  11U
#define LP_NUMBER_START		0U

#define MAX_TIME_VALUE      995U


typedef enum
{
	MODIF_SYMBOL,
	MODIF_SYMB_POZ


}alpha_state_t;

typedef enum
{
	CHOICE_PROG=0,
	CHANGE_NAME,
	SET_FREQ,
	START_PROG,
	DEFAULT,
	CHOICE_OPTION

}choice_prog_t;



typedef struct
{
	uint32_t frequency;
	uint16_t time;
}Freq_TimeParam_t;


typedef struct
{
	Freq_TimeParam_t FreqTime[MAX_FREQ_TIME_ON_PROG];
	uint8_t SymetricVoltage		:1U;
	uint8_t Reserved			:7U;

}ProgParamBase_t;

typedef struct
{
	ProgParamBase_t parameters[MAX_PROG_NAME_SIGN];
}ProgParam_t;


void menu_alphabet_500ms(switch_t curr_button, EnkoderState_t enc_status, uint8_t prog_name_nr );
extern void init_for_menu(void);
extern void menu_main(void);

#endif /* MENU_H_ */
