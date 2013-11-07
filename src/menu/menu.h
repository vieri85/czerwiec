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
//#include <stddef.h>

void menu_alphabet_500ms(switch_t curr_button,uint8_t prog_name_nr );
extern void init_for_menu(void);
#endif /* MENU_H_ */
