/*
 * app_time.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Andrzej Surowiec
 */

#ifndef APP_APP_TIME_H_
#define APP_APP_TIME_H_

#include "FreeRTOS.h"
#include "app_timer.h"
#include <time.h>

#define TICKS_PER_SEC 993

extern time_t current_time;
extern uint32_t ticks_acc;
extern struct tm time_date;
void time_handler(TickType_t ticks);
void set_date(time_t date);

#endif /* APP_APP_TIME_H_ */
