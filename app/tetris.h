/*
 * tetris.h
 *
 *  Created on: May 31, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef APP_TETRIS_H_
#define APP_TETRIS_H_

#include "buttons.h"

void draw_tetris(void);
void setinput(uint8_t in);
void tetris_handle_button_evt(button_event_t *evt);

#endif /* APP_TETRIS_H_ */
