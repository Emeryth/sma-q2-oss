/*
 * app_pairing.h
 *
 *  Created on: Mar 9, 2020
 *      Author: andrju
 */

#ifndef APP_APP_PAIRING_H_
#define APP_APP_PAIRING_H_

#include "applet.h"

void pairing_draw(void);
void pairing_handle_button_evt(button_event_t *evt);
void pairing_enter(void);

#endif /* APP_APP_PAIRING_H_ */
