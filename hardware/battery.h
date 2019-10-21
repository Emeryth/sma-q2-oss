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

void battery_init(void);
void battery_sample(void);
int battery_get_voltage(void);
uint8_t battery_get_percent(void);
uint8_t battery_get_charge_status(void);


#endif /* HARDWARE_BATTERY_H_ */
