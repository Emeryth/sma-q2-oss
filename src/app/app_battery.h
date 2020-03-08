/*
 * app_battery.h
 *
 *  Created on: Feb 7, 2020
 *      Author: andrju
 */

#ifndef APP_APP_BATTERY_H_
#define APP_APP_BATTERY_H_

#include "FreeRTOS.h"
#include "buttons.h"
#include "task.h"

void battery_process(void);
void battery_draw(void);
void battery_handle_button_evt(button_event_t* evt);

#endif /* APP_APP_BATTERY_H_ */
