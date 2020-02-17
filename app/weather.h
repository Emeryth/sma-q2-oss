/*
 * weather.h
 *
 *  Created on: Jan 14, 2020
 *      Author: andrju
 */

#ifndef APP_WEATHER_H_
#define APP_WEATHER_H_

#include <stdint.h>
#include <time.h>
#include "smaq2oss.pb.h"

#define WEATHER_FORECAST_COUNT 3

void weather_set(SetWeather *weather);
int weather_condition_to_icon(int condition);

typedef struct forecast{

	int condition;
	int8_t temperature_min;
	int8_t temperature_max;
	int8_t humidity;
	struct tm date;

}forecast_t;

typedef struct weather{

	int timestamp;
	int condition;
	int8_t temperature;
	int8_t temperature_min;
	int8_t temperature_max;
	int8_t humidity;
	forecast_t forecasts[WEATHER_FORECAST_COUNT];
	struct tm date;

}weather_t;

extern weather_t weather_current;

#endif /* APP_WEATHER_H_ */
