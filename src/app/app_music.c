/*
 * app_music.c
 *
 *  Created on: Jan 9, 2020
 *      Author: andrju
 */

#include "app_music.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "icon.h"
#include "nrf_gfx.h"
#include "vibration.h"
#include "ble_protocol.h"
#include "screen_mgr.h"

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t * title_font = &m1c_18ptbFontInfo;
static const nrf_gfx_font_desc_t * artist_font = &m1c_16ptbFontInfo;
static const nrf_gfx_font_desc_t * album_font = &m1cthin_14ptFontInfo;

enum music_control_state{
	SKIP_STATE,
	VOLUME_STATE
};

int control_state=SKIP_STATE;
music_info_t music_info;
int refresh=0;

void music_process(void){

	if (refresh){
		screen_redraw_request();
		refresh=0;
	}
}

void music_draw(void){
	lcd_clear(WHITE);

	nrf_gfx_rect_t artist_start = NRF_GFX_RECT(8, 20,140,60);
	nrf_gfx_rect_t title_start = NRF_GFX_RECT(8, 60,140,80);
	nrf_gfx_rect_t album_start = NRF_GFX_RECT(8, 128,140,60);

	nrf_gfx_print_box_utf8(p_lcd, &artist_start, BLACK, music_info.artist, artist_font, true);
    nrf_gfx_print_box_utf8(p_lcd, &title_start, BLACK, music_info.track, title_font, true);
    nrf_gfx_print_box_utf8(p_lcd, &album_start, BLACK, music_info.album, album_font, true);

	nrf_gfx_rect_t sidebar_bg=NRF_GFX_RECT(LCD_WIDTH-24,20,24,LCD_HEIGHT);
	nrf_gfx_rect_draw(p_lcd,&sidebar_bg,0,BLACK,true);

	nrf_gfx_point_t top_icon = NRF_GFX_POINT(LCD_WIDTH-20, 32);
	nrf_gfx_point_t mid_icon = NRF_GFX_POINT(LCD_WIDTH-20, 32+64);
	nrf_gfx_point_t bot_icon = NRF_GFX_POINT(LCD_WIDTH-20, 32+128);

	lcd_draw_icon(mid_icon.x, mid_icon.y, icons[ICON_PLAY]);

	if (control_state==SKIP_STATE){
		lcd_draw_icon(top_icon.x, top_icon.y, icons[ICON_REV]);
		lcd_draw_icon(bot_icon.x, bot_icon.y, icons[ICON_FWD]);
	}
	else{
		lcd_draw_icon(top_icon.x, top_icon.y, icons[ICON_VOL_UP]);
		lcd_draw_icon(bot_icon.x, bot_icon.y, icons[ICON_VOL_DOWN]);
	}

//	nrf_gfx_display(p_lcd);
}

void music_handle_button_evt(button_event_t *evt){

	if (evt->button == BUTTON_BACK && (evt->press_type == SHORT_PRESS_RELEASE|| evt->press_type == LONG_PRESS)) {
		screen_return();
	}
	else if (evt->button == BUTTON_OK && evt->press_type == LONG_PRESS){
		if (control_state==SKIP_STATE){
			control_state=VOLUME_STATE;
		}
		else{
			control_state=SKIP_STATE;
		}
	}
	else if (evt->press_type == SHORT_PRESS_RELEASE){

		switch (evt->button){
		case BUTTON_OK:
			ble_send_music_event(EVT_PLAY_PAUSE);
			break;
		case BUTTON_UP:
			if (control_state==SKIP_STATE) ble_send_music_event(EVT_REV);
			else ble_send_music_event(EVT_VOL_UP);
			break;
		case BUTTON_DOWN:
			if (control_state==SKIP_STATE) ble_send_music_event(EVT_FWD);
			else ble_send_music_event(EVT_VOL_DOWN);
			break;
		}
	}

	screen_redraw_request();
}

void music_set_music_info(music_info_t *info){

	music_info=*info;
	refresh=1;
}
