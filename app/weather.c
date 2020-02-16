/*
 * weather.c
 *
 *  Created on: Jan 14, 2020
 *      Author: andrju
 */

#include "weather.h"

weather_t weather_current={0};

static time_t _time;

void weather_set(SetWeather *weather){

	weather_current.timestamp=weather->timestamp;
	weather_current.condition=weather->condition;
	weather_current.temperature=weather->temperature;
	weather_current.temperature_min=weather->temperature_min;
	weather_current.temperature_max=weather->temperature_max;
	weather_current.humidity=weather->humidity;

	_time=weather_current.timestamp;
	localtime_r(&_time,&weather_current.date);

	for (int i=0;i<WEATHER_FORECAST_COUNT;i++){
		weather_current.forecasts[i].condition=weather->forecasts[i].condition;
		weather_current.forecasts[i].temperature_min=weather->forecasts[i].temperature_min;
		weather_current.forecasts[i].temperature_max=weather->forecasts[i].temperature_max;

		_time+=24*60*60;
		localtime_r(&_time,&weather_current.forecasts[i].date);
	}
}
