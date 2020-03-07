/*
 * app_battery.c
 *
 *  Created on: Feb 7, 2020
 *      Author: andrju
 */
#include "app_battery.h"
#include "battery.h"
#include "lcd.h"
#include "icon.h"
#include "nrf_gfx.h"
#include "screen_mgr.h"

#define GRID_H_STEP 24
#define GRID_V_STEP 20

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t * font = &m1cthin_14ptFontInfo;

char battery_string[32];

void battery_process(void){

}

void battery_draw(void){

	lcd_clear(BLACK);

	nrf_gfx_point_t volt_start = NRF_GFX_POINT(8, 24);

	int volts=battery_get_voltage();
	int percent=battery_get_percent();
	snprintf(battery_string,32,"%dmV %d%%",volts,percent);
    nrf_gfx_print(p_lcd, &volt_start, WHITE, battery_string, font, true);


    // Draw grid

    nrf_gfx_point_t graph_bot_left= NRF_GFX_POINT(10, 150);
    nrf_gfx_point_t graph_top_right= NRF_GFX_POINT(10+BATTERY_HISTORY_LENGTH+16, 150-100);

    nrf_gfx_line_t grid_v_line=NRF_GFX_LINE(graph_bot_left.x,graph_top_right.y,graph_bot_left.x,graph_bot_left.y,1);
    for (int i=0;i<BATTERY_HISTORY_LENGTH+GRID_H_STEP;i+=GRID_H_STEP){
    	nrf_gfx_line_draw(p_lcd,&grid_v_line,WHITE);
    	grid_v_line.x_start+=GRID_H_STEP;
    	grid_v_line.x_end+=GRID_H_STEP;
    }

    nrf_gfx_line_t grid_h_line=NRF_GFX_LINE(graph_bot_left.x,graph_top_right.y,graph_top_right.x,graph_top_right.y,1);
    for (int i=0;i<=100;i+=GRID_V_STEP){
    	nrf_gfx_line_draw(p_lcd,&grid_h_line,WHITE);
    	grid_h_line.y_start+=GRID_V_STEP;
    	grid_h_line.y_end+=GRID_V_STEP;
    }

    // Draw graph

    nrf_gfx_line_t graph_line=NRF_GFX_LINE(graph_bot_left.x,graph_top_right.y,graph_bot_left.x,graph_bot_left.y,1);
    for (int i=0;i<BATTERY_HISTORY_LENGTH;i++){
    	graph_line.y_start=graph_top_right.y+(100-battery_history[i]);
    	nrf_gfx_line_draw(p_lcd,&graph_line,GREEN);
    	graph_line.x_start+=1;
    	graph_line.x_end+=1;
    }
}

void battery_handle_button_evt(button_event_t *evt){

	if (evt->button == BUTTON_BACK && (evt->press_type == SHORT_PRESS_RELEASE|| evt->press_type == LONG_PRESS)) {
		screen_return();
	}

	screen_redraw_request();
}
