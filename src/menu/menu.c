/*
 * menu.c
 *
 *  Created on: Jul 23, 2013
 *      Author: Bono
 */

#include "menu.h"
#include "nvm_init.h"



const uint8_t empty_line[] = {"                "};
uint8_t header_change_name[] = "USTAW NAZWE";
uint8_t header_choice_prog_to_set[] = "WYBIERZ PROGRAM";
uint8_t header_parametry[] = "PARAMETRY";
uint8_t header_choice_option[] = "MENU";
uint8_t header_start_prog[] = "START PROGRAM";
uint8_t header_czestot_set[]= "USTAW CZESTOTLIWOSC";
uint8_t header_default_set[]= "RESETUJ USTAWIENIA";
uint8_t par_name[] = "Nazwa:";
uint8_t par_freq[] = "LP F[Hz]  T[min]";
uint8_t par_atlitude[] = "Amplituda";

uint8_t empty_sent[] = " ";
uint8_t mod_poz[] = "pozycja";
uint8_t mod_letter[] = "litera";

uint8_t change_name[]= "Name:";


uint8_t alphabet_basic[]="ABCDEFGHIJKLMNOPRSTUWXYZ0123456789@#$%^&*_ "; /*0 - 42 (ALPHABET_LENGTH)signs + NULL*/

uint8_t vert_list_arr_up_1st[] =  "'";
uint8_t vert_list_arr_down_1st[] = "~";
uint8_t vert_list_hash_1st[] = 	{0x22,0x00};

uint8_t select_sign[] = ">";


/************************PROGRAM PARAMETERS **************************************************************/

uint8_t* prog_name_all = &MRam.data.prog_name[0][0];

ProgParam_t paramy;

ProgParam_t* prog_param = (ProgParam_t*)&MRam.data.parameters_programs.parameters[0].FreqTime[0].frequency;

uint8_t*  option_list[OPTION_NUMBER] ={header_choice_prog_to_set, header_change_name, header_czestot_set, header_parametry,header_default_set, header_start_prog};

/*********************************************************************************************************/


/*menu state variable*/
main_menu_t main_menu_state = SET_PARAM;

line_operation_t change_prog_name;


void show_alphabet(u8 alpha_curr_symb, menu_line_nr_t line_nr);
void schow_vertical_list(uint8_t* header, uint8_t* list_ptr, uint8_t curr_nr, uint8_t max_nr);
void ShowVertListNoNum(uint8_t* header, uint8_t** list_ptr, uint8_t curr_nr, uint8_t max_nr);
void Menu_GenArrowAsciiNumAndDot(uint8_t* ptr_to_buf,uint8_t number,uint8_t sign);
uint8_t ChoiceProgToSetOrRun(EnkoderState_t enc_status);
void prog_param_menu(void);
static void Menu_GenAsciiNum(uint8_t* ptr_to_buf,uint8_t number);
static void Menu_GenFreqNum(uint8_t* ptr_to_buf,uint32_t number);
//void freq_list( uint8_t curr_prog,uint8_t currY_pos, uint8_t currX_pos);
void freq_listNEW( uint8_t curr_prog,uint8_t currY_pos, uint8_t currX_pos, ProgParam_t* prog_param);
static void Menu_GenTimeNum(uint8_t* ptr_to_buf, uint16_t number);
void SetFrequency(switch_t curr_button,EnkoderState_t enc_status,uint8_t prog_number, ProgParam_t *prog_param);

void init_for_menu(void)
{
	change_prog_name.line_nr = LINE_2;
	change_prog_name.mark_symb_nr=5;
	change_prog_name.mark_type = VANISH;
	change_prog_name.sentence_in = change_name;
	change_prog_name.sentence_med = prog_name_all;
	change_prog_name.sentence_out = empty_sent;

	prog_param->parameters[0].FreqTime[0].frequency = 12345;
}

/*
 * menu_alphabet_500ms
 * Function used to operate in sub menu "Change name"
 * Take care about change letter in prog name and shift alphabet left/right.
 */

void prog_param_menu(void)
{
	switch_t curr_button;
	static choice_prog_t menu_state = CHOICE_OPTION;
	static choice_prog_t menu_header_num = CHOICE_PROG;
	static uint8_t prog_number=0;
	uint16_t enkoder_value=0;
	EnkoderState_t enc_status = STABLE;
	volatile FLASH_Status status ;

	curr_button = read_any_button();
	enkoder_value = read_encoder();

	if(enkoder_value > 0x0100)
	{
		enc_status = INCREMENT;
		set_encoder(0xFF);
	}
	else if(enkoder_value < 0xFE)
	{
		enc_status = DECREMENT;
		set_encoder(0xFF);
	}

	if(ESC == curr_button)
	{
		if(menu_state == CHANGE_NAME)
		{
			status = FLASH_GetStatus();
			NVM_SimpleUpdate();
		}
		menu_state = CHOICE_OPTION;
	}

	if(CHOICE_OPTION == menu_state)
	{

		if(ENTER == curr_button)
		{
			menu_state = menu_header_num;
		}
		else if((enc_status == DECREMENT)&&(menu_header_num < (CHOICE_OPTION - 1)))
		{
			++menu_header_num;
		}
		else if((enc_status == INCREMENT)&&(menu_header_num > CHOICE_PROG))
		{
			--menu_header_num;
		}

		ShowVertListNoNum( header_choice_option , option_list, menu_header_num, OPTION_NUMBER);

	}

	/*MAIN MENU state machine*/
	switch(menu_state)
	{
	case CHOICE_PROG:
		prog_number = ChoiceProgToSetOrRun(enc_status);
		break;

	case CHANGE_NAME:
		menu_alphabet_500ms(curr_button, enc_status, prog_number);
		break;

	case SET_FREQ:
		SetFrequency(curr_button,enc_status,prog_number, prog_param);
		break;

	case START_PROG:

		menu_state = CHOICE_OPTION;
		break;

	case DEFAULT:
		FLASH_OB_Unlock();
		FLASH_Unlock();
		(void)FLASH_ErasePage(EEPROM_ADRESS_BANK_0);
		(void)FLASH_ErasePage(EEPROM_ADRESS_BANK_1);
		(void)FLASH_ErasePage(EEPROM_ADRESS_BANK_2);
		(void)FLASH_ErasePage(EEPROM_ADRESS_BANK_3);
		FLASH_Lock();
		MRam_InitDefault();
		NVM_SimpleUpdate();
		menu_state = CHOICE_OPTION;
		break;
	case CHOICE_OPTION:
		break;


	default:
		break;
	}
}

void SetFrequency(switch_t curr_button,EnkoderState_t enc_status,uint8_t prog_number, ProgParam_t *prog_param)
{
	static uint8_t x_cursor = LP_START_POSITION;
	static uint8_t y_cursor = LP_NUMBER_START;
	uint32_t prescaler_u32;
	uint8_t curr_pos_value;

	/* Determine X cursor */
	if(LEFT == curr_button)
	{
		/*Move cursor position left*/

		if(x_cursor <= FREQ_START_POSITION)
		    x_cursor = LP_START_POSITION;
		else if(x_cursor >= TIME_ONLY_POSITION)
			x_cursor = FREQ_END_POSITION;
		else
			--x_cursor;
	}
	else if(RIGHT == curr_button)
	{
		if(x_cursor >= FREQ_END_POSITION)
		{
			x_cursor = TIME_ONLY_POSITION;
		}
		else if(x_cursor <= LP_START_POSITION)
		{
			x_cursor = FREQ_START_POSITION;
		}
		else
		{
			++x_cursor;
		}
	}


	/*Determine Y position*/
	if(x_cursor == LP_START_POSITION)
	{
		if((enc_status == INCREMENT) && (y_cursor < (MAX_FREQ_TIME_ON_PROG-1)))
		{
			++y_cursor;
		}
		else if((enc_status == DECREMENT) && (y_cursor > LP_NUMBER_START))
		{
			--y_cursor;
		}
	}
	/*Determine frequency value*/
	else if((x_cursor >= FREQ_START_POSITION) && (x_cursor <= FREQ_END_POSITION))
	{

		switch((FREQ_END_POSITION - x_cursor))
		{
		case 0:
			prescaler_u32 = 1;
			break;
		case 1:
			prescaler_u32 = 10;
			break;
		case 2:
			prescaler_u32 = 100;
			break;
		case 3:
			prescaler_u32 = 1000;
			break;
		case 4:
			prescaler_u32 = 10000;
			break;
		case 5:
			prescaler_u32 = 100000;
			break;
		default:
			prescaler_u32 = 0;
			break;
		}

		curr_pos_value = ((prog_param->parameters[prog_number].FreqTime[y_cursor].frequency)/prescaler_u32) % 10;

		if((enc_status == INCREMENT) && (curr_pos_value<9))
		{
			/*Add to frequency only multiple 10 and position frequency current changing*/
			prog_param->parameters[prog_number].FreqTime[y_cursor].frequency += prescaler_u32;
		}
		else if((enc_status == DECREMENT) && (curr_pos_value > 0))
		{
			prog_param->parameters[prog_number].FreqTime[y_cursor].frequency -= prescaler_u32;
		}
	}
	/*Determine time value*/
	else if(x_cursor == TIME_ONLY_POSITION)
	{
		if((enc_status == INCREMENT) && (prog_param->parameters[prog_number].FreqTime[y_cursor].time < MAX_TIME_VALUE))
		{
			/*Add to frequency only multiple 10 and position frequency current changing*/
			prog_param->parameters[prog_number].FreqTime[y_cursor].time += 5;
		}
		else if((enc_status == DECREMENT) && (prog_param->parameters[prog_number].FreqTime[y_cursor].time > 4))
		{
			prog_param->parameters[prog_number].FreqTime[y_cursor].time -= 5;
		}
	}

	freq_listNEW(prog_number, y_cursor, x_cursor, prog_param);
}


uint8_t ChoiceProgToSetOrRun(EnkoderState_t enc_status)
{
	static uint8_t curr_prog_name=0;



	switch(enc_status)
	{

		case DECREMENT:

			if(curr_prog_name > 0)
			{
				--curr_prog_name;
			}
			break;

		case INCREMENT:

			if(curr_prog_name < (MAX_PROG_NAME_SIGN-1))
			{
				++curr_prog_name;;
			}
			break;

		default:
			break;
	}

	schow_vertical_list( header_choice_prog_to_set , prog_name_all, curr_prog_name, 10);

	return curr_prog_name;
}


void menu_alphabet_500ms(switch_t curr_button,EnkoderState_t enc_status, uint8_t prog_name_nr )
{
	static uint8_t alpha_curr_symb=0;
	static uint8_t mark_counter_100ms=0;
	static uint8_t mark_number = MARK_NAME_OFFSET;
	line_operation_t line_show;

	switch(enc_status)
	{

		case INCREMENT:

			if(mark_number < LINE_LENGTH)
			{
				++mark_number;
			}
			break;

		case DECREMENT:

			if(mark_number > MARK_NAME_OFFSET)
			{
				--mark_number;
			}
			break;

		default:
			break;

	}

	switch(curr_button)
	{
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

		default:
			break;
	}

	/*Modification symbol in current program name at position determine by mark_symbol
	* Symbol is change on current alphabet position symbol
	*/
	prog_name_all[(prog_name_nr * PROG_NUM_ELEMNT_NUM)+(mark_number-MARK_NAME_OFFSET)] = alphabet_basic[alpha_curr_symb];

	if(mark_counter_100ms > 3 && mark_counter_100ms <= 6)
	{
		line_show.mark_type = HASH;
	}
	else if(mark_counter_100ms <= 3)
	{
		line_show.mark_type = NORMAL;
	}
	else
	{
		mark_counter_100ms = 0;
	}
	mark_counter_100ms++;


	menu_header_put(header_change_name);

	line_show.sentence_med = &prog_name_all[prog_name_nr * PROG_NUM_ELEMNT_NUM];
	line_show.sentence_in = par_name;
	line_show.line_nr = LINE_2;
	line_show.sentence_out = NULL;
	line_show.mark_symb_nr = mark_number;
	menu_line_put(line_show);

	/* Clear line nr 3 */
	lcd_sentence(empty_line, LINE_3, 0 ,NORMAL);

	show_alphabet(alpha_curr_symb, LINE_4);

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


	lcd_sentence(alpha_tab_loc, line_nr,8, INVERT );
}


void schow_vertical_list(uint8_t* header, uint8_t* list_ptr, uint8_t curr_nr, uint8_t max_nr)
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
		line_ptr_2.word_descr[1].sentence_ptr = &list_ptr[first_line_refer * PROG_NUM_ELEMNT_NUM];
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
		line_ptr_3.word_descr[1].sentence_ptr = &list_ptr[(first_line_refer+1) * PROG_NUM_ELEMNT_NUM];
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
		line_ptr_4.word_descr[1].sentence_ptr = &list_ptr[(first_line_refer+2) * PROG_NUM_ELEMNT_NUM];
		menu_line_put_ext(line_ptr_4);
	}
}


void ShowVertListNoNum(uint8_t* header, uint8_t** list_ptr, uint8_t curr_nr, uint8_t max_nr)
{
	LineOperationExtend_t line_ptr_2; //{empty_sent,empty_sent,empty_sent,LINE_2,1,NORMAL};
	LineOperationExtend_t line_ptr_3; //= {empty_sent,empty_sent,empty_sent,LINE_3,1,NORMAL};
	LineOperationExtend_t line_ptr_4;// = {empty_sent,empty_sent,empty_sent,LINE_4,1,NORMAL};
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

	line_ptr_2.word_descr[1].start_pos=1;
	line_ptr_3.word_descr[1].start_pos=1;
	line_ptr_4.word_descr[1].start_pos=1;



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
		if(sign != 0)
		{
			line_ptr_2.word_descr[0].sentence_ptr = select_sign;
		}
		else
		{
			line_ptr_2.word_descr[0].sentence_ptr = empty_sent;
		}

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
		if(sign != 0)
		{
			line_ptr_3.word_descr[0].sentence_ptr = select_sign;
		}
		else
		{
			line_ptr_3.word_descr[0].sentence_ptr = empty_sent;
		}

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
		if(sign != 0)
		{
			line_ptr_4.word_descr[0].sentence_ptr = select_sign;
		}
		else
		{
			line_ptr_4.word_descr[0].sentence_ptr = empty_sent;
		}
		line_ptr_4.word_descr[1].sentence_ptr = list_ptr[first_line_refer+2];
		menu_line_put_ext(line_ptr_4);
	}
}


void freq_listNEW( uint8_t curr_prog,uint8_t currY_pos, uint8_t currX_pos, ProgParam_t *prog_param )
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
	     *Fields			AABCDEFGHIJKLLLM
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
		line_ptr.mark_symb_nr = currX_pos;


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
			/*Determine which field will be change to set INVERT mark type*/

			line_ptr.mark_type = INVERT;
		}
		else
		{
			line_ptr.mark_type = NORMAL;
		}

		/*FIELD C - Calculate to ASCII series a FREQUENCY value*/
		Menu_GenFreqNum(freq_show,prog_param->parameters[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param->parameters[curr_prog].FreqTime[curr_refer_line].time);

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
			/*Determine which field will be change to set INVERT mark type*/
			line_ptr.mark_type = INVERT;

		}
		else
		{
			line_ptr.mark_type = NORMAL;
		}

		/*FIELD C Calculate to ASCII series frequency value*/
		Menu_GenFreqNum(freq_show,prog_param->parameters[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param->parameters[curr_prog].FreqTime[curr_refer_line].time);

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
			/*Determine which field will be change to set INVERT mark type*/
			line_ptr.mark_type = INVERT;
		}
		else
		{
			line_ptr.mark_type = NORMAL;
		}


		/*FIELD C Calculate to ASCII series frequency value*/
		Menu_GenFreqNum(freq_show,prog_param->parameters[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param->parameters[curr_prog].FreqTime[curr_refer_line].time);

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


#if 0
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
		Menu_GenFreqNum(freq_show,prog_param.parameters[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param.parameters[curr_prog].FreqTime[curr_refer_line].time);

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
		Menu_GenFreqNum(freq_show,prog_param.parameters[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param.parameters[curr_prog].FreqTime[curr_refer_line].time);

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
		Menu_GenFreqNum(freq_show,prog_param.parameters[curr_prog].FreqTime[curr_refer_line].frequency);

		/*FIELD E - Calculate to ASCII series a TIME value*/
		Menu_GenTimeNum(time_show, prog_param.parameters[curr_prog].FreqTime[curr_refer_line].time);

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

#endif



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

	if(number <= MAX_TIME_VALUE)
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

