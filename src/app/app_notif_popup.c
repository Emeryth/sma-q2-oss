/*
 * app_notif_popup.c
 *
 *  Created on: Mar 5, 2020
 *      Author: andrju
 */

#include "app_notif_popup.h"
#include "lcd.h"
#include "notification.h"
#include "vibration.h"
#include "screen_mgr.h"
#include "app_notification.h"

static int8_t current_item_index=0;
static 	int num_items=0;

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t * sender_font = &m1c_12ptbFontInfo;
static const nrf_gfx_font_desc_t * text_font = &m1c_12ptFontInfo;

static void next_notification(void){

	current_item_index++;
//	num_items = notification_get_unread_count();

	if (current_item_index>=num_items){
		current_item_index=0;
	}
}

static void previous_notification(void){

	current_item_index--;
//	num_items = notification_get_unread_count();

	if (current_item_index<0){
		current_item_index=num_items-1;
	}
}

void notif_popup_process(void){

	if (num_items<notification_get_unread_count()){
		vibration_notification();
		current_item_index=0;
		screen_redraw_request();
	}

}

void notif_popup_draw(void) {

	num_items = notification_get_unread_count();
	int last = notification_get_count();

//	nrf_gfx_rect_t sender_text = NRF_GFX_RECT(24,24,LCD_WIDTH-32,20);
//	nrf_gfx_rect_t body_text = NRF_GFX_RECT(4,24+20,LCD_WIDTH-8,LCD_HEIGHT-64);
//	nrf_gfx_point_t item_icon = NRF_GFX_POINT(4,24+4);
//
//	notification_decode(notification_get(last - current_item_index - 1));
//
//	nrf_gfx_print_box_utf8(p_lcd, &sender_text, BLACK, notification_buf.sender,
//			sender_font, true);
//	nrf_gfx_print_box_utf8(p_lcd, &body_text, BLACK, notification_buf.body,
//			text_font, true);
//
//	lcd_draw_icon(item_icon.x, item_icon.y, icons[ICON_MESSAGE]);

	notification_draw_full(notification_get(last - current_item_index - 1));

	nrf_gfx_rect_t footer = NRF_GFX_RECT(0,LCD_HEIGHT-20,LCD_WIDTH,20);
	nrf_gfx_rect_draw(p_lcd,&footer,1,BLACK,true);

	nrf_gfx_point_t footer_text = NRF_GFX_POINT(LCD_WIDTH/2,LCD_HEIGHT-20);
	char pages[10];
	snprintf(pages,sizeof(pages),"%d/%d",current_item_index+1,num_items);
	nrf_gfx_print(p_lcd, &footer_text, WHITE, pages, text_font, true);

}

void notif_popup_handle_button_evt(button_event_t *evt){
	if (evt->button == BUTTON_BACK && (evt->press_type == SHORT_PRESS_RELEASE|| evt->press_type == LONG_PRESS)) {
		screen_return();
	}
	else if (evt->button == BUTTON_UP && evt->press_type == SHORT_PRESS_RELEASE) {
		previous_notification();
		screen_redraw_request();
	}
	else if (evt->button == BUTTON_DOWN && evt->press_type == SHORT_PRESS_RELEASE) {
		next_notification();
		screen_redraw_request();
	}
}

void notif_popup_enter(void){
	current_item_index=0;
	vibration_notification();
}

void notif_popup_exit(void){
	notification_clear_unread();
}
