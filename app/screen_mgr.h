/*
 * screen_mgr.h
 *
 *  Created on: Jan 15, 2020
 *      Author: andrju
 */

#ifndef APP_SCREEN_MGR_H_
#define APP_SCREEN_MGR_H_

#include "buttons.h"
#include "applet.h"

void screen_manage(void);
void screen_handle_button_evt(button_event_t *evt);
void screen_redraw_request();
void screen_switch(int id);
void screen_return(void);
void screen_mgr_init(void);


#endif /* APP_SCREEN_MGR_H_ */
