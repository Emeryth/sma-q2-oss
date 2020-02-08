/*
 * icons.c
 *
 *  Created on: Oct 11, 2019
 *      Author: Andrzej Surowiec
 */

#include "icon.h"


icon_t icons[NUM_ICONS]={

		{.id=ICON_HEART_BIG,.x=0,.y=0,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_HEART_SMALL,.x=2,.y=0,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_BLUETOOTH,.x=4,.y=0,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_BATTERY,.x=0,.y=2,.w=3,.h=2,.data=icons_dat},
		{.id=ICON_CHARGE,.x=15,.y=2,.w=1,.h=2,.data=icons_dat},
		{.id=ICON_PLAY,.x=0,.y=6,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_FWD,.x=2,.y=6,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_REV,.x=4,.y=6,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_VOL_DOWN,.x=6,.y=6,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_VOL_UP,.x=8,.y=6,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_TETRIS,.x=10,.y=6,.w=2,.h=2,.data=icons_dat},
		{.id=ICON_BATTERY_MENU,.x=6,.y=2,.w=3,.h=2,.data=icons_dat},

};

icon_t weather_icons[NUM_WEATHER_ICONS]={

		{.id=WEATHER_SUN,.x=0,.y=0,.w=4,.h=4,.data=weather_dat},
		{.id=WEATHER_CLOUD,.x=4,.y=0,.w=4,.h=4,.data=weather_dat},
		{.id=WEATHER_SUN_CLOUD,.x=8,.y=0,.w=4,.h=4,.data=weather_dat},

};
