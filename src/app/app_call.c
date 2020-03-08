/*
 * app_call.c
 *
 *  Created on: Feb 16, 2020
 *      Author: andrju
 */

#include "app_call.h"
#include "ble_protocol.h"
#include "lcd.h"
#include "vibration.h"
#include <string.h>

call_info_t call;

static const nrf_lcd_t* p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t* name_font = &m1c_18ptbFontInfo;
static const nrf_gfx_font_desc_t* number_font = &m1cthin_14ptFontInfo;

void call_process(void)
{

    if (call_get_status() != CALL_INCOMING) {
        screen_return();
    }
}

void call_draw(void)
{

    lcd_clear(WHITE);

    nrf_gfx_rect_t status_start = NRF_GFX_RECT(8, 20, 140, 60);
    nrf_gfx_rect_t name_start = NRF_GFX_RECT(8, 60, 140, 80);
    nrf_gfx_rect_t number_start = NRF_GFX_RECT(8, 150, 140, 60);

    nrf_gfx_print_box_utf8(p_lcd, &name_start, BLACK, call.name, name_font, true);
    nrf_gfx_print_box_utf8(p_lcd, &number_start, BLACK, call.number, number_font, true);

    nrf_gfx_rect_t sidebar_bg = NRF_GFX_RECT(LCD_WIDTH - 24, 20, 24, LCD_HEIGHT);
    nrf_gfx_rect_draw(p_lcd, &sidebar_bg, 0, BLACK, true);

    nrf_gfx_point_t top_icon = NRF_GFX_POINT(LCD_WIDTH - 20, 32);
    nrf_gfx_point_t mid_icon = NRF_GFX_POINT(LCD_WIDTH - 20, 32 + 64);
    nrf_gfx_point_t bot_icon = NRF_GFX_POINT(LCD_WIDTH - 20, 32 + 128);

    lcd_draw_icon(top_icon.x, top_icon.y, icons[ICON_OK]);
    lcd_draw_icon(bot_icon.x, bot_icon.y, icons[ICON_CANCEL]);
}

void call_handle_button_evt(button_event_t* evt)
{

    if (evt->press_type == SHORT_PRESS_RELEASE) {

        switch (evt->button) {
        case BUTTON_BACK:
            call.command = CALL_IDLE;
            screen_return();
            break;
        case BUTTON_UP:
            call.command = CALL_ACCEPT;
            ble_send_call_command(CALL_ACCEPT);
            screen_return();
            break;
        case BUTTON_DOWN:
            call.command = CALL_REJECT;
            ble_send_call_command(CALL_REJECT);
            screen_return();
            break;
        }
    }
}

void call_enter(void)
{
    vibration_call();
}

void call_exit(void)
{
    vibration_stop();
}

void call_set_info(CallNotification* notif)
{

    if (notif->command == CALL_INCOMING) {
        call.command = notif->command;
        memcpy(call.name, notif->name, CALL_NAME_MAX_LEN);
        memcpy(call.number, notif->number, CALL_NUMBER_MAX_LEN);
    } else {
        call.command = notif->command;
    }
}

int call_get_status(void)
{
    return call.command;
}
