/*
 * keyboard.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Bono
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include <stdint.h>

typedef enum
{
	ENTER=1,
	ESC=2,
	LEFT=3,
	RIGHT=4,
	UNDEF=5
}switch_t;


typedef union
{
	struct
	{
		uint32_t enter:1;
		uint32_t esc:1;
		uint32_t left:1;
		uint32_t right:1;
		uint32_t reserved:28;
	}button;

	uint32_t all;
}buttons_state_t;

typedef enum
{
	KEY_RESET=0,
	KEY_SET=1,
	KEY_UNDEF=2
}buton_state_t;


extern buton_state_t read_button_status(switch_t switch_nr);
extern void Button_Set(switch_t switch_nr);
extern switch_t read_any_button(void);

#endif /* KEYBOARD_H_ */
