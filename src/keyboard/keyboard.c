/*
 * keyboard.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Bono
 */
#include "keyboard.h"

static switch_t global_button_state;

void Button_Set(switch_t switch_nr)
{
	global_button_state = switch_nr;
}

/*After read status some button will be clear
 *
 */
//buton_state_t read_button_status(switch_t switch_nr)
//{
//	uint8_t ret;
//	switch(switch_nr)
//	{
//	case ENTER:
//		ret = global_button_state.button.enter;
//		break;
//	case ESC:
//		ret = global_button_state.button.esc;
//		break;
//	case LEFT:
//		ret = global_button_state.button.left;
//		break;
//	case RIGHT:
//		ret = global_button_state.button.right;
//		break;
//	default:
//		/*Wrong request*/
//		ret = UNDEF;
//		break;
//	}
//	/*After read some button state and it was SET
//	 * then all other will be reset to avoid unpredictable
//	 * behaviour of change menu
//	 */
//	if(ret == 1)
//	{
//		global_button_state.all = 0;
//	}
//
//	return ret;
//}

switch_t read_any_button(void)
{
	switch_t ret;
   ret = global_button_state;
   global_button_state = 0;

   return ret;
}
