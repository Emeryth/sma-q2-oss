/*
 * watchface.c
 *
 *  Created on: Nov 3, 2018
 *      Author: Andrzej Surowiec
 */

#include "watchface.h"
#include "battery.h"
#include "tetris.h"
#include "pah8002.h"
#include "status.h"
#include "weather.h"
#include "screen_mgr.h"
#include "vibration.h"

static const nrf_gfx_font_desc_t * p_font = &m1cthin_12ptFontInfo;
static const nrf_gfx_font_desc_t * p_digit_font = &m1mn_48ptFontInfo;
static const nrf_gfx_font_desc_t * p_weather_font = &m1c_18ptbFontInfo;
static const nrf_gfx_font_desc_t * p_weather_font_small = &m1c_14ptbFontInfo;
static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;

static char time_str[32];

void watchface_process(void){

}

void watchface_draw(void) {
	lcd_clear(WHITE);

	nrf_gfx_point_t digits_start = NRF_GFX_POINT(32,30);
	nrf_gfx_point_t date_start = NRF_GFX_POINT(32,90);

	snprintf(time_str, sizeof(time_str), "%02d:%02d", time_date.tm_hour,
			time_date.tm_min);
	nrf_gfx_print(p_lcd, &digits_start, BLACK, time_str, p_digit_font, true);

//    asctime_r(&time_date,time_str);
//    sprintf(time_str,"%u\n%u", (uint)current_time, (uint)ticks_acc);
	strftime(time_str, sizeof(time_str), "%a %d.%m.%Y", &time_date);
	nrf_gfx_print(p_lcd, &date_start, BLACK, time_str, p_font, true);

	nrf_gfx_rect_t weather_bg=NRF_GFX_RECT(0,120,LCD_WIDTH,76);
	nrf_gfx_rect_draw(p_lcd,&weather_bg,0,BLACK,true);

	nrf_gfx_point_t temperature_start = NRF_GFX_POINT(64,128);
	nrf_gfx_point_t temperature_max_start = NRF_GFX_POINT(120,124);
	nrf_gfx_point_t temperature_min_start = NRF_GFX_POINT(120,146);

	lcd_draw_icon(24, 128, weather_icons[weather_condition_to_icon(weather_current.condition)]);

	snprintf(time_str, sizeof(time_str), "%d\xB0",weather_current.temperature);
	nrf_gfx_print(p_lcd, &temperature_start, WHITE, time_str, p_weather_font, true);

	snprintf(time_str, sizeof(time_str), "%d\xB0",weather_current.temperature_max);
	nrf_gfx_print(p_lcd, &temperature_max_start, WHITE, time_str, p_weather_font_small, true);
	snprintf(time_str, sizeof(time_str), "%d\xB0",weather_current.temperature_min);
	nrf_gfx_print(p_lcd, &temperature_min_start, WHITE, time_str, p_weather_font_small, true);

//	nrf_gfx_display(p_lcd);
}

void draw_statusbar(uint8_t draw_time) {

	nrf_gfx_rect_t statusbar_bg=NRF_GFX_RECT(0,0,LCD_WIDTH,20);
	nrf_gfx_rect_draw(p_lcd,&statusbar_bg,0,BLACK,true);

	nrf_gfx_point_t text_start = NRF_GFX_POINT(72, 2);

	if (draw_time) {
		snprintf(time_str, sizeof(time_str), "%02d:%02d", time_date.tm_hour,
				time_date.tm_min);
		nrf_gfx_print(p_lcd, &text_start, WHITE, time_str, p_font, true);
	}

	uint8_t batt = battery_get_percent();

	lcd_draw_icon(LCD_WIDTH - 32, 0, icons[ICON_BATTERY]);
	uint8_t bar_w = batt*10/55;
	nrf_gfx_rect_t bar=NRF_GFX_RECT(LCD_WIDTH - 32+4+(18-bar_w),2,bar_w,12);
	int color;
	if (batt>50){
		color=GREEN;
	}
	else if(batt>20){
		color=YELLOW;
	}
	else{
		color=RED;
	}
	nrf_gfx_rect_draw(p_lcd,&bar,0,color,true);

	if (battery_get_charge_status()!=BATTERY_DISCHARGING){
		lcd_draw_icon(LCD_WIDTH - 24, 0, icons[ICON_CHARGE]);
	}
	else{
//		text_start.y=1;
//		text_start.x=LCD_WIDTH - 26;
//		snprintf(time_str,sizeof(time_str),"%02d",battery_get_percent());
//		nrf_gfx_print(p_lcd, &text_start, WHITE, time_str, p_font, true);
		int batt = battery_get_percent();
		lcd_draw_small_num(LCD_WIDTH - 26,4,(batt/10)%10);
		lcd_draw_small_num(LCD_WIDTH - 20,4,batt%10);
	}

	if (status_get_ble_connected()) {
		lcd_draw_icon(LCD_WIDTH - 56, 0, icons[ICON_BLUETOOTH]);
	}
}

void watchface_handler(void *p_context) {

//	draw_watchface();
//	draw_tetris();

}

void watchface_handle_button_evt(button_event_t *evt) {

	if (evt->button == BUTTON_UP && evt->press_type == LONG_PRESS) {
		screen_switch(APPLET_MUSIC);
	}
	else if (evt->button == BUTTON_OK && evt->press_type == SHORT_PRESS_RELEASE) {
		screen_switch(APPLET_MENU);
	}
	else if (evt->button == BUTTON_DOWN && evt->press_type == SHORT_PRESS_RELEASE) {
		screen_switch(APPLET_WEATHER);
	}
//	else if (evt->button == BUTTON_UP && evt->press_type == SHORT_PRESS_RELEASE) {
//		vibration_call();
//	}
	else if (evt->button == BUTTON_BACK && evt->press_type == SHORT_PRESS_RELEASE) {
		vibration_stop();
	}

	screen_redraw_request();
}
