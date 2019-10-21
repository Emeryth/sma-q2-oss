/*
 * lcd.h
 *
 *  Created on: Oct 20, 2018
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_LCD_H_
#define HARDWARE_LCD_H_

#include "nrf_gfx.h"
#include "icon.h"

#define LPM013M126A_HEIGHT 176
#define LPM013M126A_WIDTH 176

#define LCD_HEIGHT LPM013M126A_HEIGHT
#define LCD_WIDTH LPM013M126A_WIDTH

#define LCD_LINE_SIZE (LCD_WIDTH/2)
#define LCD_BUFFER_SIZE (LCD_HEIGHT*LCD_LINE_SIZE)

enum lcd_colors{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PINK,
	YELLOW,
	WHITE

};

extern const nrf_gfx_font_desc_t orkney_24ptFontInfo;
extern const nrf_gfx_font_desc_t orkney_8ptFontInfo;
extern const nrf_gfx_font_desc_t dSEG7ClassicMini_32ptFontInfo;
extern const nrf_lcd_t nrf_lcd_lpm013m126a;

extern uint8_t lcd_buffer[LCD_HEIGHT][LCD_LINE_SIZE];

void lcd_clear(uint32_t color);
void lcd_clear_lines(uint8_t start, uint8_t end,uint32_t color);
void lcd_draw_icon(uint8_t x,uint8_t y, icon_t icon);
void lcd_draw_small_num(uint8_t x,uint8_t y, uint8_t num);

#endif /* HARDWARE_LCD_H_ */
