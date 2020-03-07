/*
 * weather.c
 *
 *  Created on: Jan 14, 2020
 *      Author: andrju
 */

#include "weather.h"
#include "icon.h"
#include "screen_mgr.h"

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
	screen_redraw_request();
}

int weather_condition_to_icon(int condition){

	// Based on https://openweathermap.org/weather-conditions
	if (condition>=200&&condition<=232){
		return WEATHER_CLOUDS_THUNDER;
	}
	if (condition>=300&&condition<=321){
		return WEATHER_CLOUDS_RAIN;
	}
	if (condition>=500&&condition<=504){
		return WEATHER_SUN_CLOUD_RAIN;
	}
	if (condition>=511&&condition<=511){
		return WEATHER_SNOW;
	}
	if (condition>=520&&condition<=531){
		return WEATHER_CLOUDS_RAIN;
	}
	if (condition>=600&&condition<=622){
		return WEATHER_SNOW;
	}
	if (condition>=700&&condition<=781){
		return WEATHER_MIST;
	}
	if (condition>=800&&condition<=800){
		return WEATHER_SUN;
	}
	if (condition>=801&&condition<=801){
		return WEATHER_SUN_CLOUD;
	}
	if (condition>=802&&condition<=802){
		return WEATHER_CLOUD;
	}
	if (condition>=803&&condition<=804){
		return WEATHER_CLOUDS;
	}


	return WEATHER_SUN;
}
