/*
 * app_pairing.c
 *
 *  Created on: Mar 9, 2020
 *      Author: andrju
 */


#include "app_pairing.h"
#include "lcd.h"
#include "ble_gap.h"
#include "ble_init.h"
#include "status.h"
#include "screen_mgr.h"

extern uint8_t passkey[BLE_GAP_PASSKEY_LEN + 1];

static const nrf_gfx_font_desc_t * passkey_font = &m1c_18ptbFontInfo;
static const nrf_gfx_font_desc_t * info_font = &m1c_16ptbFontInfo;
static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;

void pairing_draw(void){
	lcd_clear(WHITE);

	nrf_gfx_rect_t info_start = NRF_GFX_RECT(8, 20,140,60);
	nrf_gfx_rect_t passkey_start = NRF_GFX_RECT(8, 60,140,80);

	nrf_gfx_print_box_utf8(p_lcd, &info_start, BLACK, "Pairing code", info_font, true);
    nrf_gfx_print_box_utf8(p_lcd, &passkey_start, BLACK, passkey, passkey_font, true);

	nrf_gfx_rect_t sidebar_bg=NRF_GFX_RECT(LCD_WIDTH-24,20,24,LCD_HEIGHT);
	nrf_gfx_rect_draw(p_lcd,&sidebar_bg,0,BLACK,true);

	nrf_gfx_point_t top_icon = NRF_GFX_POINT(LCD_WIDTH-20, 32);
	nrf_gfx_point_t bot_icon = NRF_GFX_POINT(LCD_WIDTH-20, 32+128);

	lcd_draw_icon(top_icon.x, top_icon.y, icons[ICON_OK]);
	lcd_draw_icon(bot_icon.x, bot_icon.y, icons[ICON_CANCEL]);

}

void pairing_enter(void){
	status_set_pairing_request(false);
}

void pairing_handle_button_evt(button_event_t *evt){

	 if (evt->press_type == SHORT_PRESS_RELEASE){

			switch (evt->button){
			case BUTTON_BACK:
				screen_return();
				break;
			case BUTTON_UP:
				ble_accept_passkey();
				screen_return();
				break;
			case BUTTON_DOWN:
				ble_reject_passkey();
				screen_return();
				break;
			}
		}

}
