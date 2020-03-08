/*
 * app_menu.h
 *
 *  Created on: Jan 15, 2020
 *      Author: andrju
 */

#ifndef APP_APP_MENU_H_
#define APP_APP_MENU_H_

#include "applet.h"
#include "icon.h"
#include "lcd.h"
#include "nrf_gfx.h"

#define NUM_MENU_ITEMS 4
#define MENU_ITEMS_PER_PAGE 4

#define MENU_START_ROW 20
#define MENU_HEIGHT (LCD_HEIGHT - MENU_START_ROW)
#define MENU_ITEM_HEIGHT (MENU_HEIGHT / MENU_ITEMS_PER_PAGE)

typedef struct menu_item {
    uint8_t name[16];
    icon_t* icon;
    int applet_id;

} menu_item_t;

extern menu_item_t menu_items[NUM_MENU_ITEMS];

void menu_process(void);
void menu_draw(void);
void menu_handle_button_evt(button_event_t* evt);

#endif /* APP_APP_MENU_H_ */
