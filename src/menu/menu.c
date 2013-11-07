/*
 * menu.c
 *
 *  Created on: Jul 23, 2013
 *      Author: Bono
 */

#include "menu.h"


#define MAX_PROG_NAME_SIGN		10U
#define MARK_NAME_OFFSET 		5U
#define PROG_NAME_LENGTH 		10U
#define LINE_LENGTH 			15U
#define ALPHABET_LENGTH 		42U
#define ALPHA_POS_OFFSET		8U
#define MAX_FREQ_TIME_ON_PROG	15U
#define PROGRAM_NUMBER			10U

/*Frequency and time selector*/
#define LP_NR				0U
#define ARROW_SELECT_FREQ	1U
#define FREQ_VALUE			2U
#define ARROW_SELECT_TIME	3U
#define TIME_VALUE			4U
#define PROGRES_SIGN		5U


typedef enum
{
	MODIF_SYMBOL,
	MODIF_SYMB_POZ


}alpha_state_t;

typedef enum
{
	CHOICE_PROG,
	CHANGE_NAME,
	SET_FREQ

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

}ProgParam_t;


uint8_t header_change_name[] = "SET NEW NAME";
uint8_t header_choice_prog_to_set[] = "WybierzProgram";
uint8_t header_parametry[] = "Parametry";

uint8_t par_name[] = "Nazwa";
uint8_t par_freq[] = "LP F[Hz]  T[min]";
uint8_t par_atlitude[] = "Amplituda";


uint8_t empty_sent[] = " ";
uint8_t mod_poz[] = "pozycja";
uint8_t mod_letter[] = "litera";

uint8_t change_name[]= "Name:";
uint8_t prog_name_1[] ="PROGRAM_01";
uint8_t prog_name_2[] ="PROGRAM_02";

uint8_t alphabet_basic[]="ABCDEFGHIJKLMNOPRSTUWXYZ0123456789@#$%^&*_ "; /*0 - 42 (ALPHABET_LENGTH)signs + NULL*/

uint8_t vert_list_arr_up_1st[] =  "'";
uint8_t vert_list_arr_down_1st[] = "~";
uint8_t vert_list_hash_1st[] = 	{0x22,0x00};

uint8_t select_sign[] = ">";


/************************PROGRAM PARAMETERS **************************************************************/

uint8_t * prog_name_all[PROGRAM_NUMBER] ={prog_name_1,prog_name_2,prog_name_2,prog_name_2,prog_name_2,
							  prog_name_2,prog_name_2,prog_name_2,prog_name_2,prog_name_2};

ProgParam_t prog_param[PROGRAM_NUMBER];


/*********************************************************************************************************/


/*menu state variable*/
main_menu_t main_menu_state = SET_PARAM;

line_operation_t change_prog_name;


void show_alphabet(u8 alpha_curr_symb, menu_line_nr_t line_nr);
void schow_vertical_list(uint8_t* header, uint8_t** list_ptr, uint8_t curr_nr, uint8_t max_nr);
void Menu_GenArrowAsciiNumAndDot(uint8_t* ptr_to_buf,uint8_t number,uint8_t sign);
uint8_t ChoiceProgToSetOrRun(switch_t curr_button);
void prog_param_menu(void);
static void Menu_GenAsciiNum(uint8_t* ptr_to_buf,uint8_t number);
static void Menu_GenFreqNum(uint8_t* ptr_to_buf,uint32_t number);
void freq_list( uint8_t curr_prog,uint8_t currY_pos, uint8_t currX_pos);
static void Menu_GenTimeNum(uint8_t* ptr_to_buf, uint16_t number);

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

void prog_param_menu(void)
{
	switch_t curr_button;
	static choice_prog_t choice_prog_state=CHOICE_PROG;
	static uint8_t prog_number=0;


	curr_button = read_any_button();
	switch(choice_prog_state)
	{
	case CHOICE_PROG:
		prog_param[0].FreqTime[4].frequency = 128390;
		freq_list(prog_number,5, 1);
		//prog_number = ChoiceProgToSetOrRun(curr_button);
		break;

	case CHANGE_NAME:
		//menu_alphabet_500ms(curr_button, prog_number);
		break;

	case SET_FREQ:
		break;

	default:
		break;
	}
}

uint8_t ChoiceProgToSetOrRun(switch_t curr_button)
{
	static line_operation_t choice_prog_to_set;
	static uint8_t curr_prog_name=0;



	switch(curr_button)
	{
		case ENTER:

			break;

		case LEFT:

			if(curr_prog_name >0)
			{
				--curr_prog_name;
			}
			break;

		case RIGHT:

			if(curr_prog_name < 9)
			{
				++curr_prog_name;;
			}
			break;

	}

	schow_vertical_list( header_choice_prog_to_set , prog_name_all, curr_prog_name, 10);

	return curr_prog_name;
}

#if 0
void menu_alphabet_500ms(switch_t curr_button,uint8_t prog_name_nr )
{
	static uint8_t curr_prog_name=0;
	static alpha_state_t alpha_curr_state=MODIF_SYMB_POZ;
	static uint8_t alpha_curr_symb=0;
	static uint8_t mark_counter_100ms=0;


	if(curr_button == ESC)
	{
		//back to higher menu level;
	}

	change_prog_name.sentence_med = prog_name_all[prog_name_nr];
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
			prog_name_all[prog_name_nr][change_prog_name.mark_symb_nr-MARK_NAME_OFFSET] = alphabet_basic[alpha_curr_symb];
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
#endif

void schow_vertical_list(uint8_t* header, uint8_t** list_ptr, uint8_t curr_nr, uint8_t max_nr)
{
	LineOperationExtend_t line_ptr_2; //{empty_sent,empty_sent,empty_sent,LINE_2,1,NORMAL};
	LineOperationExtend_t line_ptr_3; //= {empty_sent,empty_sent,empty_sent,LINE_3,1,NORMAL};
	LineOperationExtend_t line_ptr_4;// = {empty_sent,empty_sent,empty_sent,LINE_4,1,NORMAL};
	uint8_t number_line_2[5];
	uint8_t number_line_3[5];
	uint8_t number_line_4[5];
	uint8_t sign=0;
	uint8_t local_var;
	static uint8_t first_line_refer = 0;

	/* BE AWARE
	 * curr_nr define line to select but it position could be 2,3 or 4 line in LCD
	 * so we use "first_line_refer" to determine absolute position of list
	 */

	if(curr_nr < first_line_refer)
	{
		first_line_refer = curr_nr;
	}

	local_var = first_line_refer+2;

	if(curr_nr > local_var)
	{
		first_line_refer = curr_nr - 2;
	}

	menu_header_put(header);

	/*Arrow and Hash to indicate up down scroll*/
	line_ptr_2.line_nr = LINE_2;
	line_ptr_3.line_nr = LINE_3;
	line_ptr_4.line_nr = LINE_4;

	line_ptr_2.word_descr[2].start_pos = 15;
	line_ptr_3.word_descr[2].start_pos = 15;
	line_ptr_4.word_descr[2].start_pos = 15;

	line_ptr_2.word_descr[0].start_pos=0;
	line_ptr_3.word_descr[0].start_pos=0;
	line_ptr_4.word_descr[0].start_pos=0;

	line_ptr_2.word_descr[1].start_pos=4;
	line_ptr_3.word_descr[1].start_pos=4;
	line_ptr_4.word_descr[1].start_pos=4;

	line_ptr_2.max_ptr_nr = 2;
	line_ptr_3.max_ptr_nr = 2;
    line_ptr_4.max_ptr_nr = 2;

	if(max_nr>3)
	{
		/*Up line*/
		if(first_line_refer > 0)
		{
			line_ptr_2.word_descr[2].sentence_ptr = vert_list_arr_up_1st;
		}
		else
			line_ptr_2.word_descr[2].sentence_ptr = vert_list_hash_1st;

		/*Medium line*/
		line_ptr_3.word_descr[2].sentence_ptr = vert_list_hash_1st;


		local_var = max_nr - first_line_refer;

		/*Down line*/
		if(local_var < 4)
		{
			line_ptr_4.word_descr[2].sentence_ptr = vert_list_hash_1st;
		}
		else
		{
			line_ptr_4.word_descr[2].sentence_ptr = vert_list_arr_down_1st;
		}

	}

	local_var = curr_nr - first_line_refer;

	/*Line 2 determine if must be '>'*/
	if(local_var == 0)
	{
		sign=1;
	}
	else
	{
		sign =0;
	}

	if(max_nr>0)
	{
		Menu_GenArrowAsciiNumAndDot(number_line_2, first_line_refer, sign);
		line_ptr_2.word_descr[0].sentence_ptr = number_line_2;
		line_ptr_2.word_descr[1].sentence_ptr = list_ptr[first_line_refer];
		menu_line_put_ext(line_ptr_2);
	}


	/*Line 3 determine if must be '>'*/
	if(local_var == 1)
	{
		sign=1;
	}
	else
	{
		sign =0;
	}

	if(max_nr>1)
	{
		Menu_GenArrowAsciiNumAndDot(number_line_3,(first_line_refer+1),sign);
		line_ptr_3.word_descr[0].sentence_ptr= number_line_3;
		line_ptr_3.word_descr[1].sentence_ptr = list_ptr[first_line_refer+1];
		menu_line_put_ext(line_ptr_3);
	}


	/*Line 4 determine if must be '>'*/
	if(local_var == 2)
	{
		sign=1;
	}
	else
	{
		sign =0;
	}
	if(max_nr>2)
	{
		Menu_GenArrowAsciiNumAndDot(number_line_4,(first_line_refer+2),sign);
		line_ptr_4.word_descr[0].sentence_ptr= number_line_4;
		line_ptr_4.word_descr[1].sentence_ptr = list_ptr[first_line_refer+2];
		menu_line_put_ext(line_ptr_4);
	}
}


void freq_list( uint8_t curr_prog,uint8_t currY_pos, uint8_t currX_pos)
{
		LineOperationExtend_t line_ptr;
		uint8_t number_line[3];
		uint8_t freq_show[7];
		uint8_t time_show[5];
		uint8_t curr_refer_line=0;

		uint8_t local_var;
		static uint8_t first_line_refer = 0;

		/*HOW IT LOOK LIKE*/

		/*Header			LP F[Hz]  T[min]
		 *Line_2			03 000585  15.0
		 *Line_3			04 001580 >10.5
		 *Line_4			05 999995  01.5
	     *
	     *Fields			AABCCCCCC DEEEEF
		 */


		/* BE AWARE
		 * curr_nr define line to select but it position could be 2,3 or 4 line in LCD
		 * so we use "first_line_refer" to determine absolute position of list
		 */

		if(currY_pos < first_line_refer)
		{
			first_line_refer = currY_pos;
		}

		local_var = first_line_refer+2;

		if(currY_pos > local_var)
		{
			first_line_refer = currY_pos - 2;
		}

		menu_header_put(par_freq);

		/*Determine start position cursor for various field on the line*/
		line_ptr.word_descr[LP_NR].start_pos = 0;
		line_ptr.word_descr[ARROW_SELECT_FREQ].start_pos = 2;
		line_ptr.word_descr[FREQ_VALUE].start_pos = 3;
		line_ptr.word_descr[ARROW_SELECT_TIME].start_pos = 10;
		line_ptr.word_descr[TIME_VALUE].start_pos = 11;
		line_ptr.word_descr[PROGRES_SIGN].start_pos = 15;

		/*Determine constant pointers and value for line generator structure*/

		line_ptr.max_ptr_nr = 5;
		line_ptr.word_descr[LP_NR].sentence_ptr = number_line;
		line_ptr.word_descr[FREQ_VALUE].sentence_ptr = freq_show;
		line_ptr.word_descr[TIME_VALUE].sentence_ptr = time_show;
		line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = NULL;
		line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = NULL;



	/*Up line***************************************************/
		/*Select line to operate*/
		line_ptr.line_nr = LINE_2;

		/*Current table operator for the line*/
		curr_refer_line = first_line_refer;

		/*FIELD A -  Calculate to ASCII series lap position for line*/
		Menu_GenAsciiNum(number_line,curr_refer_line);

		/*FIELD B and D -   Check if this line is currently selected */
		local_var = currY_pos - curr_refer_line;
		if(local_var == 0)
		{
			/*Determine if '>' must select frequency or time field*/
			if(currX_pos == 0)
			{
				line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = select_sign;
				line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = NULL;
			}
			else
			{
				line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = select_sign;
				line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = NULL;
			}
		}

		/*FIELD C - Calculate to ASCII series a FREQUENCY value*/
		Menu_GenFreqNum(freq_show,prog_param[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param[curr_prog].FreqTime[curr_refer_line].time);

	    /*Field F -   Check if shall be write ^ or # on the end of first text line*/
		if(first_line_refer > 0)
		{
			line_ptr.word_descr[PROGRES_SIGN].sentence_ptr = vert_list_arr_up_1st;
		}
		else
		{
			line_ptr.word_descr[PROGRES_SIGN].sentence_ptr = vert_list_hash_1st;
		}

		/*Create LINE 2*/
		menu_line_put_ext(line_ptr);




	/*Medium line*********************************************/

		line_ptr.line_nr = LINE_3;

		/*Current table operator for the line*/
		curr_refer_line = first_line_refer + 1;

		/*FIELD A -  Calculate to ASCII series lap position for line*/
		Menu_GenAsciiNum(number_line,curr_refer_line);

		/*FIELD B and D -   Check if this line is currently selected */
		local_var = currY_pos - curr_refer_line;
		if(local_var == 0)
		{
			/*Determine if '>' must select frequency or time field*/
			if(currX_pos == 0)
			{
				line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = select_sign;
				line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = NULL;
			}
			else
			{
				line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = select_sign;
				line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = NULL;
			}
		}

		/*FIELD C Calculate to ASCII series frequency value*/
		Menu_GenFreqNum(freq_show,prog_param[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param[curr_prog].FreqTime[curr_refer_line].time);

	    /*Field F -   write # on the end of text line*/
		line_ptr.word_descr[PROGRES_SIGN].sentence_ptr = vert_list_hash_1st;


		/*Create LINE 3*/
		menu_line_put_ext(line_ptr);



	/*Down line**********************************************/

		line_ptr.line_nr = LINE_4;

		/*Current table operator for the line*/
		curr_refer_line = first_line_refer + 2;

		/*FIELD A -  Calculate to ASCII series lap position for line*/
		Menu_GenAsciiNum(number_line,curr_refer_line);

		/*FIELD B and D -   Check if this line is currently selected and*/
		local_var = currY_pos - curr_refer_line;
		if(local_var == 0)
		{
			/*Determine if '>' must select frequency or time field*/
			if(currX_pos == 0)
			{
				line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = select_sign;
				line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = NULL;
			}
			else
			{
				line_ptr.word_descr[ARROW_SELECT_TIME].sentence_ptr = select_sign;
				line_ptr.word_descr[ARROW_SELECT_FREQ].sentence_ptr = NULL;
			}
		}


		/*FIELD C Calculate to ASCII series frequency value*/
		Menu_GenFreqNum(freq_show,prog_param[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param[curr_prog].FreqTime[curr_refer_line].time);

	    /*Field F -   Check if shall be write ^ or # on the end of first text line*/
		local_var = MAX_FREQ_TIME_ON_PROG - first_line_refer;
		if(local_var < 4)
		{
			line_ptr.word_descr[PROGRES_SIGN].sentence_ptr = vert_list_hash_1st;
		}
		else
		{
			line_ptr.word_descr[PROGRES_SIGN].sentence_ptr = vert_list_arr_down_1st;
		}

		/*Create LINE 4*/
		menu_line_put_ext(line_ptr);

}





void Menu_GenArrowAsciiNumAndDot(uint8_t* ptr_to_buf,uint8_t number,uint8_t sign)
{
	++number;
	if(number<100)
	{
		ptr_to_buf[1] = (number/10)+48;
		ptr_to_buf[2] = (number%10)+48;
	}
	else
	{
		ptr_to_buf[1] = '#';
		ptr_to_buf[2] = '#';
	}

	if(sign>0)
	{
		ptr_to_buf[0] = '>';
	}
	else
	{
		ptr_to_buf[0] = ' ';
	}

	ptr_to_buf[3] = '.';
	ptr_to_buf[4] = 0; /*Null at the end of sentence*/


}

static void Menu_GenAsciiNum(uint8_t* ptr_to_buf,uint8_t number)
{
	++number;
	if(number<100)
	{
		ptr_to_buf[0] = (number/10)+48;
		ptr_to_buf[1] = (number%10)+48;
	}
	else
	{
		ptr_to_buf[1] = '#';
		ptr_to_buf[2] = '#';
	}

	ptr_to_buf[2] = 0; /*Null at the end of sentence*/


}

static void Menu_GenFreqNum(uint8_t* ptr_to_buf, uint32_t number)
{

	uint32_t loc_var;
	uint8_t a=6;
	if(number<1000000U)
	{


		for(;a>0;a--)
		{
			loc_var = number%10;
			number = number/10;
			ptr_to_buf[a-1] = loc_var + '0';
		}

	}
	else
	{
		ptr_to_buf[0] = '#';
		ptr_to_buf[1] = '#';
	}

	ptr_to_buf[6] = 0; /*Null at the end of sentence*/


}

static void Menu_GenTimeNum(uint8_t* ptr_to_buf, uint16_t number)
{

	/*Time has resolution 5U, 5 is equal 30sec*
	 * time_max=995 its mean 99,5 min*/
	uint16_t loc_var=0;

	if(number<=995)
	{


		loc_var = number%10;
		number = number/10;
		ptr_to_buf[3] = loc_var + '0';

		loc_var = number%10;
		number = number/10;
		ptr_to_buf[1] = loc_var + '0';


		loc_var = number%10;
		ptr_to_buf[0] = loc_var + '0';

		ptr_to_buf[2] = '.';

	}
	else
	{
		ptr_to_buf[0] = '#';
		ptr_to_buf[1] = '#';
	}

	ptr_to_buf[4] = 0; /*Null at the end of sentence*/
}


void menu_main(void)
{
	static uint8_t k=0;
	if(k >= 1)
	{
		k=0;
		prog_param_menu();
	}
	k++;

}

