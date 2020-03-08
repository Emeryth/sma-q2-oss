/*
 * app_weather.c
 *
 *  Created on: Feb 17, 2020
 *      Author: andrju
 */

#include "app_weather.h"
#include "lcd.h"
#include "weather.h"

static const nrf_lcd_t* p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t* date_font = &m1c_10ptbFontInfo;
static const nrf_gfx_font_desc_t* temperature_font = &m1c_14ptbFontInfo;

#define WEATHER_ROW_STEP 48

void weather_draw(void)
{

    char string[16];

    lcd_clear(BLACK);

    nrf_gfx_rect_t date_start = NRF_GFX_RECT(16, 32, 64, 64);
    nrf_gfx_point_t icon_start = NRF_GFX_POINT(16 + 48, 32);
    nrf_gfx_point_t temperature_max_start = NRF_GFX_POINT(112, 24);
    nrf_gfx_point_t temperature_min_start = NRF_GFX_POINT(112, 24 + 20);

    for (int i = 0; i < WEATHER_FORECAST_COUNT; i++) {

        strftime(string, sizeof(string), "%a\n%d/%m", &weather_current.forecasts[i].date);
        nrf_gfx_print_box_utf8(p_lcd, &date_start, WHITE, string, date_font, true);

        lcd_draw_icon(icon_start.x, icon_start.y, weather_icons[weather_condition_to_icon(weather_current.forecasts[i].condition)]);

        snprintf(string, sizeof(string), "%d\xB0", weather_current.forecasts[i].temperature_max);
        nrf_gfx_print(p_lcd, &temperature_max_start, WHITE, string, temperature_font, true);
        snprintf(string, sizeof(string), "%d\xB0", weather_current.forecasts[i].temperature_min);
        nrf_gfx_print(p_lcd, &temperature_min_start, WHITE, string, temperature_font, true);

        date_start.y += WEATHER_ROW_STEP;
        icon_start.y += WEATHER_ROW_STEP;
        temperature_max_start.y += WEATHER_ROW_STEP;
        temperature_min_start.y += WEATHER_ROW_STEP;
    }
}

void weather_handle_button_evt(button_event_t* evt)
{

    if (evt->button == BUTTON_UP && evt->press_type == SHORT_PRESS_RELEASE) {
        screen_return();
    } else if (evt->button == BUTTON_BACK && evt->press_type == SHORT_PRESS_RELEASE) {
        screen_return();
    }
}
