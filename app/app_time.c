/*
 * app_time.c
 *
 *  Created on: Nov 3, 2018
 *      Author: Andrzej Surowiec
 */


#include "app_time.h"

time_t current_time;
struct tm time_date;
uint32_t ticks_acc;

void time_handler(TickType_t ticks){

	static uint32_t previous_ticks=0;

	uint32_t current_ticks;
	uint32_t ticks_diff;

	current_ticks=ticks;
	ticks_diff=current_ticks-previous_ticks;

	ticks_acc+=ticks_diff;
	previous_ticks=current_ticks;

	if (ticks_acc<TICKS_PER_SEC){
		return;
	}

	while (ticks_acc>=TICKS_PER_SEC){

		ticks_acc-=TICKS_PER_SEC;
		current_time++;

	}

	localtime_r(&current_time,&time_date);


}

void set_date(time_t date){

	current_time=date;

}

bool time_is_older_than_24h(time_t date){

	if (current_time-SECS_PER_DAY>date){
		return true;
	}

	return false;

}
