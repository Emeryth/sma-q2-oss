/*
 * app_notif_popup.h
 *
 *  Created on: Mar 5, 2020
 *      Author: andrju
 */

#ifndef APP_APP_NOTIF_POPUP_H_
#define APP_APP_NOTIF_POPUP_H_

#include "applet.h"

void notif_popup_process(void);
void notif_popup_draw(void);
void notif_popup_handle_button_evt(button_event_t *evt);
void notif_popup_enter(void);
void notif_popup_exit(void);

#endif /* APP_APP_NOTIF_POPUP_H_ */
