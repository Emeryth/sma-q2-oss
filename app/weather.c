/*
 * weather.c
 *
 *  Created on: Jan 14, 2020
 *      Author: andrju
 */

#include "weather.h"

weather_t weather_current={0};

void weather_set(uint8_t *data){

	weather_current.condition=data[1];
	weather_current.temperature=data[2];
	weather_current.temperature_min=data[3];
	weather_current.temperature_max=data[4];
	weather_current.humidity=data[5];

}
