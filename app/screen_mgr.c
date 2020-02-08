/*
 * screen_mgr.c
 *
 *  Created on: Jan 15, 2020
 *      Author: andrju
 */

#include "screen_mgr.h"
#include "nrf_gfx.h"
#include "lcd.h"
#include "watchface.h"

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;

int redraw_requested;
applet_t current_screen;

void screen_manage(void){

	current_screen.process_applet();

	if (redraw_requested){
		current_screen.draw_applet();

		if (current_screen.id==APPLET_WATCHFACE){
			draw_statusbar(0);
		}
		else{
			draw_statusbar(1);
		}
		nrf_gfx_display(p_lcd);
		redraw_requested=0;
	}

}

void screen_handle_button_evt(button_event_t *evt){

//	if (evt->button == BUTTON_BACK && (evt->press_type == SHORT_PRESS_RELEASE|| evt->press_type == LONG_PRESS)) {
//		screen_return();
//	}
//	else{
		current_screen.handle_button_evt(evt);
//	}

}

void screen_redraw_request(){

	redraw_requested=1;

}

void screen_switch(int id){

	if (current_screen.exit_applet!=NULL){
		current_screen.exit_applet();
	}

	int previous_id = current_screen.id;
	current_screen=applets[id];
	current_screen.previous_applet=previous_id;

	if (current_screen.enter_applet!=NULL){
		current_screen.enter_applet();
	}

	screen_redraw_request();
}

void screen_return(void){

	if (current_screen.exit_applet!=NULL){
		current_screen.exit_applet();
	}

	current_screen=applets[current_screen.previous_applet];

	if (current_screen.enter_applet!=NULL){
		current_screen.enter_applet();
	}

	screen_redraw_request();
}

void screen_mgr_init(void){

	current_screen=applets[APPLET_WATCHFACE];
	redraw_requested=1;

}
