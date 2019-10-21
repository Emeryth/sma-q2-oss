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

static const nrf_gfx_font_desc_t * p_font = &orkney_8ptFontInfo;
static const nrf_gfx_font_desc_t * p_digit_font = &dSEG7ClassicMini_32ptFontInfo;
static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;

static char time_str[32];

void draw_watchface(void) {

//    lcd_clear(BLACK);
	nrf_gfx_point_t text_start = NRF_GFX_POINT(16,54);

	snprintf(time_str, sizeof(time_str), "%02d:%02d", time_date.tm_hour,
			time_date.tm_min);
	nrf_gfx_print(p_lcd, &text_start, BLUE, time_str, p_digit_font, true);

//    asctime_r(&time_date,time_str);
//    sprintf(time_str,"%u\n%u", (uint)current_time, (uint)ticks_acc);
	strftime(time_str, sizeof(time_str), "%a %d.%m.%Y", &time_date);
	text_start.y += 56;
	text_start.x = 32;
	nrf_gfx_print(p_lcd, &text_start, WHITE, time_str, p_font, true);

	text_start.y += 16;
	int voltage = battery_get_voltage();
	int vd = voltage / 1000;
	int vf = voltage / 10 % 100;
//    snprintf(time_str,sizeof(time_str),"battery: %01d.%02dV",vd,vf);
//    snprintf(time_str,sizeof(time_str),"battery: %d",battery_get_percent());
//    nrf_gfx_print(p_lcd, &text_start, WHITE, time_str, p_font, true);

//    text_start.y+=16;
//    snprintf(time_str,sizeof(time_str),"pah8002: %x",pah8002_get_status());
//    nrf_gfx_print(p_lcd, &text_start, WHITE, time_str, p_font, true);
//
//    text_start.y+=16;
//    snprintf(time_str,sizeof(time_str),"HR: %.2f",pah8002_hr());
//    nrf_gfx_print(p_lcd, &text_start, WHITE, time_str, p_font, true);

//    lcd_draw_icon(0,0, ICON_HEART_BIG);
	nrf_gfx_display(p_lcd);
}

void draw_statusbar(uint8_t draw_time) {

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

}
