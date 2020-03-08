/*
 * app_notification.c
 *
 *  Created on: Feb 27, 2020
 *      Author: andrju
 */

#include "app_notification.h"
#include "app_time.h"
#include "lcd.h"
#include "screen_mgr.h"
#include <time.h>

#define NOTIFICATIONS_PER_PAGE 3

#define NOTIFICATIONS_START_ROW 20
#define NOTIFICATIONS_HEIGHT (LCD_HEIGHT-NOTIFICATIONS_START_ROW)
#define NOTIFICATION_ITEM_HEIGHT (NOTIFICATIONS_HEIGHT/NOTIFICATIONS_PER_PAGE)

enum draw_modes{
	DRAW_LIST,
	DRAW_NOTIFICATION
};

static int draw_mode=DRAW_LIST;

static int notifications_count=0;
static int8_t top_item_index=0;
static int8_t current_item_index=0;
static 	int num_items;
static char date_text[8];
static struct tm date;

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t * sender_font = &m1c_16ptbFontInfo;
static const nrf_gfx_font_desc_t * text_font = &m1c_12ptFontInfo;
static const nrf_gfx_font_desc_t * date_font = &m1c_12ptbFontInfo;

void notification_process(void){

}

static void notification_draw_list(void){

	lcd_clear(WHITE);

	int y=NOTIFICATIONS_START_ROW;
	num_items = (notifications_count<NOTIFICATIONS_PER_PAGE)?notifications_count:NOTIFICATIONS_PER_PAGE;

	for (int i=0;i<num_items;i++){

		int item=top_item_index+i;
		int selected = (item==current_item_index);
		nrf_gfx_rect_t item_text = NRF_GFX_RECT(4,y+16,LCD_WIDTH,NOTIFICATION_ITEM_HEIGHT-16);
		nrf_gfx_point_t item_icon = NRF_GFX_POINT(4,y+4);
		nrf_gfx_point_t item_date = NRF_GFX_POINT(4+20,y);
		nrf_gfx_rect_t item_sender = NRF_GFX_RECT(70,y,LCD_WIDTH,20);

		nrf_gfx_rect_t item_box=NRF_GFX_RECT(0,y,LCD_WIDTH,NOTIFICATION_ITEM_HEIGHT);
		nrf_gfx_rect_draw(p_lcd,&item_box,1,BLACK,selected);

		notification_decode(notification_get(notifications_count-1-item));

		int text_color;
		if (selected) text_color=WHITE;
		else text_color=BLACK;

		localtime_r((const time_t*)&notification_buf.timestamp,&date);

		if (time_is_older_than_24h(notification_buf.timestamp)){
			snprintf(date_text, sizeof(date_text), "%02d/%02d",date.tm_mday,date.tm_mon+1);
		}
		else{
			snprintf(date_text, sizeof(date_text), "%02d:%02d",date.tm_hour,date.tm_min);
		}

		nrf_gfx_print(p_lcd, &item_date, text_color, date_text, date_font, true);
		nrf_gfx_print_box_utf8(p_lcd, &item_sender, text_color, notification_buf.sender, date_font, true);
		nrf_gfx_print_box_utf8(p_lcd, &item_text, text_color, notification_buf.body, text_font, true);
//		nrf_gfx_print(p_lcd, &item_text, text_color, menu_items[item].name, p_font, true);
		lcd_draw_icon(item_icon.x, item_icon.y, icons[ICON_MESSAGE]);

		y+=NOTIFICATION_ITEM_HEIGHT;
	}

}

void notification_draw(void){

	notifications_count=notification_get_count();

	if (draw_mode==DRAW_LIST){
		notification_draw_list();
	}
	else if (draw_mode==DRAW_NOTIFICATION){
		notification_draw_full(notification_get(notifications_count-1-current_item_index));

		nrf_gfx_rect_t footer = NRF_GFX_RECT(0,LCD_HEIGHT-20,LCD_WIDTH,20);
		nrf_gfx_rect_draw(p_lcd,&footer,1,BLACK,true);

		nrf_gfx_point_t footer_text = NRF_GFX_POINT(LCD_WIDTH/2,LCD_HEIGHT-20);
		char pages[10];
		snprintf(pages,sizeof(pages),"%d/%d",current_item_index+1,notifications_count);
		nrf_gfx_print(p_lcd, &footer_text, WHITE, pages, text_font, true);
	}

}

static void next_item(void){

	current_item_index++;

	if (current_item_index-top_item_index>=NOTIFICATIONS_PER_PAGE){
		top_item_index++;
	}

	if (current_item_index>=notifications_count){
		current_item_index=0;
		top_item_index=0;
	}
}

static void previous_item(void){

	current_item_index--;

	if (current_item_index<0){
		current_item_index=notifications_count-1;
		top_item_index=notifications_count-NOTIFICATIONS_PER_PAGE;
	}

	if (current_item_index<top_item_index){
		top_item_index--;
	}
}

void notification_handle_button_evt(button_event_t *evt){
	if (evt->button == BUTTON_BACK && (evt->press_type == SHORT_PRESS_RELEASE|| evt->press_type == LONG_PRESS)) {

		if (draw_mode==DRAW_NOTIFICATION){
			draw_mode=DRAW_LIST;
			screen_redraw_request();
		}
		else{
			screen_return();
		}
	}
	else if (evt->button == BUTTON_UP && evt->press_type == SHORT_PRESS_RELEASE) {
		previous_item();
		screen_redraw_request();
	}
	else if (evt->button == BUTTON_DOWN && evt->press_type == SHORT_PRESS_RELEASE) {
		next_item();
		screen_redraw_request();
	}
	else if (evt->button == BUTTON_OK && evt->press_type == SHORT_PRESS_RELEASE) {
		draw_mode=DRAW_NOTIFICATION;
		screen_redraw_request();
	}
}

void notification_enter(void){

	current_item_index=0;
	top_item_index=0;
	draw_mode=DRAW_LIST;

}

void notification_exit(void){

}

void notification_draw_full(notification_t *notif){
	lcd_clear(WHITE);

	nrf_gfx_rect_t sender_text = NRF_GFX_RECT(24,24,LCD_WIDTH-32,20);
	nrf_gfx_rect_t body_text = NRF_GFX_RECT(4,24+20,LCD_WIDTH-8,LCD_HEIGHT-64);
	nrf_gfx_point_t item_icon = NRF_GFX_POINT(4,24+4);

	notification_decode(notif);

	nrf_gfx_print_box_utf8(p_lcd, &sender_text, BLACK, notification_buf.sender,
			sender_font, true);
	nrf_gfx_print_box_utf8(p_lcd, &body_text, BLACK, notification_buf.body,
			text_font, true);

	lcd_draw_icon(item_icon.x, item_icon.y, icons[ICON_MESSAGE]);

	nrf_gfx_rect_t footer = NRF_GFX_RECT(0,LCD_HEIGHT-20,LCD_WIDTH,20);
	nrf_gfx_rect_draw(p_lcd,&footer,1,BLACK,true);

}
