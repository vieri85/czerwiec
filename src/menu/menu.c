/*
 * menu.c
 *
 *  Created on: Jul 23, 2013
 *      Author: Bono
 */


#include "menu.h"
#include "keyboard.h"
#include <inttypes.h>
#include "ST7565R.h"
#include <stddef.h>

#define MAX_PROG_NAME_SIGN		10U
#define MARK_NAME_OFFSET 		5U
#define PROG_NAME_LENGTH 		10U
#define LINE_LENGTH 			15U
#define ALPHABET_LENGTH 		42U
#define ALPHA_POS_OFFSET		8U
typedef enum
{
	MODIF_SYMBOL,
	MODIF_SYMB_POZ


}alpha_state_t;

uint8_t header_change_name[] = "SET NEW NAME";
uint8_t empty_sent[] = " ";
uint8_t mod_poz[] = "pozycja";
uint8_t mod_letter[] = "litera";

uint8_t change_name[]= "Name:";
uint8_t prog_name_1[] ="PROGRAM_01";
uint8_t prog_name_2[] ="PROGRAM_02";

uint8_t alphabet_basic[]="ABCDEFGHIJKLMNOPRSTUWXYZ0123456789@#$%^&*_ "; /*0 - 42 (ALPHABET_LENGTH)signs + NULL*/

uint8_t * prog_name_all[10] ={prog_name_1,prog_name_2,prog_name_2,prog_name_2,prog_name_2,
							  prog_name_2,prog_name_2,prog_name_2,prog_name_2,prog_name_2};


/*menu state variable*/
main_menu_t main_menu_state = SET_PARAM;

line_operation_t change_prog_name;

void show_alphabet(u8 alpha_curr_symb, menu_line_nr_t line_nr);

void init_for_menu(void)
{
	change_prog_name.line_nr = LINE_2;
	change_prog_name.mark_symb_nr=5;
	change_prog_name.mark_type = VANISH;
	change_prog_name.sentence_in = change_name;
	change_prog_name.sentence_med = prog_name_1;
	change_prog_name.sentence_out = empty_sent;
}

/*
 * menu_alphabet_500ms
 * Function used to operate in sub menu "Change name"
 * Take care about change letter in prog name and shift alphabet left/right.
 */

void menu_alphabet_500ms(void)
{
	static uint8_t curr_prog_name=0;
	static alpha_state_t alpha_curr_state=MODIF_SYMB_POZ;
	static uint8_t alpha_curr_symb=0;
	static uint8_t mark_counter_100ms=0;
	switch_t curr_button;

	curr_button = read_any_button();
	if(curr_button == ESC)
	{
		//back to higher menu level;
	}

	change_prog_name.sentence_med = prog_name_all[curr_prog_name];
	switch(alpha_curr_state)
	{
		case  MODIF_SYMB_POZ :

			switch(curr_button)
			{
				case ENTER:
					alpha_curr_state = MODIF_SYMBOL;
					break;

				case LEFT:
					--change_prog_name.mark_symb_nr;
					if(change_prog_name.mark_symb_nr < MARK_NAME_OFFSET)
					{
						change_prog_name.mark_symb_nr = LINE_LENGTH;
					}
					break;

				case RIGHT:
					++change_prog_name.mark_symb_nr;
					if(change_prog_name.mark_symb_nr > LINE_LENGTH)
					{
						change_prog_name.mark_symb_nr = MARK_NAME_OFFSET;
					}
					break;

			}

			break;

		case MODIF_SYMBOL:

			switch(curr_button)
			{
				case ENTER:
					alpha_curr_state = MODIF_SYMB_POZ;
					break;

				case LEFT:
					--alpha_curr_symb;
					if(ALPHABET_LENGTH < alpha_curr_symb)
					{
						alpha_curr_symb = ALPHABET_LENGTH;
					}

					break;

				case RIGHT:
					++alpha_curr_symb;
					if(ALPHABET_LENGTH < alpha_curr_symb)
					{
						alpha_curr_symb = 0;
					}
					break;
			}

			/*Modification symbol in current program name at position determine by mark_symbol
			 * Symbol is change on current alphabet position symbol
			 */
			prog_name_all[curr_prog_name][change_prog_name.mark_symb_nr-MARK_NAME_OFFSET] = alphabet_basic[alpha_curr_symb];
			break;
	}

	if(mark_counter_100ms > 3 && mark_counter_100ms <= 6)
	{
		change_prog_name.mark_type = HASH;
	}
	else if(mark_counter_100ms <= 3)
	{
		change_prog_name.mark_type = NORMAL;
	}
	else
	{
		mark_counter_100ms = 0;
	}

	menu_header_put(header_change_name);
	menu_line_put(change_prog_name);
	show_alphabet(alpha_curr_symb, LINE_4);
	mark_counter_100ms++;

}


void show_alphabet(u8 alpha_curr_symb, menu_line_nr_t line_nr)
{
	uint8_t alpha_tab_loc[]="                ";
	uint8_t symb_poz_zero=0;
	uint8_t symb_inline=0;
	static uint8_t val;
//	static uint8_t mark_counter_100ms=0;

//	proba = alpha_curr_symb - ALPHA_POS_OFFSET;

//	symb_poz_zero = alpha_curr_symb - ALPHA_POS_OFFSET + 1;
	val = alpha_curr_symb - 8 ;

	/*Left side will overflow*/
	if(val>=0 && (val <= 42))
	{

		symb_poz_zero = alpha_curr_symb - 8 ;

	}
	else if(val)
	{
		symb_poz_zero = 42 + alpha_curr_symb -7;
	}



	while(alphabet_basic[symb_poz_zero] != 0)
	{

		if(symb_inline>=16)
		{
			break;
		}
		alpha_tab_loc[symb_inline] = alphabet_basic[symb_poz_zero];
		++symb_poz_zero;
		++symb_inline;

	}

	symb_poz_zero = 0;
	while(symb_inline<16)
	{
		alpha_tab_loc[symb_inline] = alphabet_basic[symb_poz_zero];
		++symb_poz_zero;
		++symb_inline;
	}


	lcd_sentence(alpha_tab_loc, line_nr,8, FOUR_MARK );
}



void menu_main(void)
{
	static uint8_t k=0;
	if(k >= 0)
	{
		k=0;
		menu_alphabet_500ms();
	}
	k++;

}

