/*
 * battery.h
 *
 *  Created on: May 12, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_BATTERY_H_
#define HARDWARE_BATTERY_H_

enum battery{
	BATTERY_DISCHARGING,
	BATTERY_CHARGING,
	BATTERY_CHARGED,
	BATTERY_FAULT
};

#define BATTERY_HISTORY_LENGTH 128
#define BATTERY_100P 290
#define BATTERY_0P 250

extern uint8_t battery_history[BATTERY_HISTORY_LENGTH];

void battery_init(void);
void battery_sample(void);
int battery_get_voltage(void);
uint8_t battery_get_percent(void);
uint8_t battery_get_charge_status(void);
void battery_save_history(void);


#endif /* HARDWARE_BATTERY_H_ */
