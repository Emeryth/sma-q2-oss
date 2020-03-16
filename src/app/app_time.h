/*
 * app_time.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Andrzej Surowiec
 */

#ifndef APP_APP_TIME_H_
#define APP_APP_TIME_H_

#include <time.h>
#include "app_timer.h"
#include "FreeRTOS.h"

#define TICKS_PER_SEC 993
#define SECS_PER_DAY 24*60*60

extern time_t current_time;
extern uint32_t ticks_acc;
extern struct tm time_date;
void time_handler(TickType_t ticks);
void set_date(time_t date);
bool time_is_older_than_24h(time_t date);

#endif /* APP_APP_TIME_H_ */
