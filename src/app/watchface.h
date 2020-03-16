/*
 * watchface.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Andrzej Surowiec
 */

#ifndef APP_WATCHFACE_H_
#define APP_WATCHFACE_H_

#include "app_timer.h"
#include "lcd.h"
#include "nrf_gfx.h"
#include "app_time.h"
#include "buttons.h"
#include "icon.h"

void watchface_process(void);
void watchface_draw(void);
void draw_statusbar(uint8_t draw_time);
void watchface_handler(void *p_context);
void watchface_handle_button_evt(button_event_t *evt);

#endif /* APP_WATCHFACE_H_ */
