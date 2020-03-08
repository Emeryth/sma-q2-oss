/*
 * app_menu.c
 *
 *  Created on: Jan 15, 2020
 *      Author: andrju
 */

#include "app_menu.h"
#include "screen_mgr.h"

static const nrf_gfx_font_desc_t* p_font = &m1c_18ptbFontInfo;
static const nrf_lcd_t* p_lcd = &nrf_lcd_lpm013m126a;

int8_t top_item_index = 0;
int8_t current_item_index = 0;

//static char menu_str[32];

menu_item_t menu_items[NUM_MENU_ITEMS] = {
    { "Music", &icons[ICON_PLAY], APPLET_MUSIC },
    { "HRM", &icons[ICON_HEART_BIG], APPLET_HRM },
    { "Tetris", &icons[ICON_TETRIS], APPLET_TETRIS },
    { "Battery", &icons[ICON_BATTERY_MENU], APPLET_BATTERY },

};

void menu_process(void)
{
}

void menu_draw(void)
{
    lcd_clear(WHITE);

    int y = MENU_START_ROW;
    int num_items;
    num_items = (NUM_MENU_ITEMS < MENU_ITEMS_PER_PAGE) ? NUM_MENU_ITEMS : MENU_ITEMS_PER_PAGE;

    for (int i = 0; i < num_items; i++) {

        int item = top_item_index + i;
        int selected = (item == current_item_index);
        nrf_gfx_point_t item_text = NRF_GFX_POINT(48, y + (MENU_ITEM_HEIGHT - 32) / 2);
        nrf_gfx_point_t item_icon = NRF_GFX_POINT(16, y + (MENU_ITEM_HEIGHT - 16) / 2);

        nrf_gfx_rect_t item_box = NRF_GFX_RECT(0, y, LCD_WIDTH, MENU_ITEM_HEIGHT);
        nrf_gfx_rect_draw(p_lcd, &item_box, 1, BLACK, selected);

        int text_color;
        if (selected)
            text_color = WHITE;
        else
            text_color = BLACK;
        nrf_gfx_print(p_lcd, &item_text, text_color, menu_items[item].name, p_font, true);
        lcd_draw_icon(item_icon.x, item_icon.y, *menu_items[item].icon);

        y += MENU_ITEM_HEIGHT;
    }
}

static void next_item(void)
{

    current_item_index++;

    if (current_item_index - top_item_index >= MENU_ITEMS_PER_PAGE) {
        top_item_index++;
    }

    if (current_item_index >= NUM_MENU_ITEMS) {
        current_item_index = 0;
        top_item_index = 0;
    }
}

static void previous_item(void)
{

    current_item_index--;

    if (current_item_index < 0) {
        current_item_index = NUM_MENU_ITEMS - 1;
        top_item_index = NUM_MENU_ITEMS - MENU_ITEMS_PER_PAGE;
    }

    if (current_item_index < top_item_index) {
        top_item_index--;
    }
}

void menu_handle_button_evt(button_event_t* evt)
{

    if (evt->button == BUTTON_BACK && (evt->press_type == SHORT_PRESS_RELEASE || evt->press_type == LONG_PRESS)) {
        screen_return();
    } else if (evt->button == BUTTON_UP && evt->press_type == SHORT_PRESS_RELEASE) {
        previous_item();
        screen_redraw_request();
    } else if (evt->button == BUTTON_DOWN && evt->press_type == SHORT_PRESS_RELEASE) {
        next_item();
        screen_redraw_request();
    } else if (evt->button == BUTTON_OK && evt->press_type == SHORT_PRESS_RELEASE) {
        screen_switch(menu_items[current_item_index].applet_id);
    }

    //	if (current_item_index>=MENU_ITEMS_PER_PAGE){
    //		current_item_index=0;
    //	}
}
