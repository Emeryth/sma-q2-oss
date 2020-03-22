/*
 * app_notification.h
 *
 *  Created on: Feb 27, 2020
 *      Author: andrju
 */

#ifndef APP_APP_NOTIFICATION_H_
#define APP_APP_NOTIFICATION_H_

#include "applet.h"
#include "notification.h"

void notification_process(void);
void notification_draw(void);
void notification_handle_button_evt(button_event_t *evt);
void notification_enter(void);
void notification_exit(void);
void notification_draw_full(notification_t *notif);

#endif /* APP_APP_NOTIFICATION_H_ */
