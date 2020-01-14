/*
 * weather.h
 *
 *  Created on: Jan 14, 2020
 *      Author: andrju
 */

#ifndef APP_WEATHER_H_
#define APP_WEATHER_H_

#include <stdint.h>

void weather_set(uint8_t *data);

typedef struct weather{

	uint8_t condition;
	int8_t temperature;
	int8_t temperature_min;
	int8_t temperature_max;
	int8_t humidity;

}weather_t;

extern weather_t weather_current;

#endif /* APP_WEATHER_H_ */
