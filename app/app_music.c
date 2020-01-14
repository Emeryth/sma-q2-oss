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

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t * title_font = &m1c_24ptFontInfo;
static const nrf_gfx_font_desc_t * artist_font = &m1c_16ptbFontInfo;
static const nrf_gfx_font_desc_t * album_font = &m1cthin_14ptFontInfo;

enum music_control_state{
	SKIP_STATE,
	VOLUME_STATE
};

int control_state=SKIP_STATE;

void music_draw(void){
	lcd_clear(WHITE);

	nrf_gfx_point_t artist_start = NRF_GFX_POINT(8, 24);
	nrf_gfx_point_t title_start = NRF_GFX_POINT(8, 64);
	nrf_gfx_point_t album_start = NRF_GFX_POINT(8, 128);

    nrf_gfx_print(p_lcd, &artist_start, BLACK, "Darude", artist_font, true);
    nrf_gfx_print(p_lcd, &title_start, BLACK, "Sandstorm", title_font, true);
    nrf_gfx_print(p_lcd, &album_start, BLACK, "Before The Storm", album_font, true);

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

	if (evt->button == BUTTON_OK && evt->press_type == LONG_PRESS){
		if (control_state==SKIP_STATE){
			control_state=VOLUME_STATE;
		}
		else{
			control_state=SKIP_STATE;
		}
	}

}
