/*
 * lcd.c
 *
 *  Created on: Oct 20, 2018
 *      Author: Andrzej Surowiec
 */

#include "sdk_common.h"

#include "nrf_lcd.h"
#include "nrf_drv_spi.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"
#include "sma-q2.h"
#include "lcd.h"
#include <wchar.h>


static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(LCD_SPI_INSTANCE);

uint8_t lcd_update_command[] = {0x90,0x01};
uint8_t lcd_noupdate_command[] = {0xA0,0x00};
uint8_t lcd_nextline[] = {0x00,0x00};

uint8_t lcd_buffer[LCD_HEIGHT][LCD_LINE_SIZE]={0};

static inline uint8_t to_4bit_color(uint32_t color){

	uint8_t color4=((color&0x07)<<1)|0x01;
	return (color4<<4)|color4;

}

void lcd_clear(uint32_t color){

	memset(lcd_buffer,to_4bit_color(color),LCD_BUFFER_SIZE);

}

void lcd_clear_lines(uint8_t start, uint8_t end,uint32_t color){

	memset(lcd_buffer+start*LCD_LINE_SIZE,to_4bit_color(color),(end-start)*LCD_LINE_SIZE);

}

void lcd_draw_pixel(uint16_t x, uint16_t y, uint8_t color){

	uint8_t pixel=lcd_buffer[y][x/2];
	uint8_t mask;

	if (x&1){
		mask=0x0F;
	}
	else{
		mask=0xF0;
	}

	lcd_buffer[y][x/2]= pixel ^ ((pixel ^ color) & mask);
}

inline void lcd_draw_8pix(uint8_t x,uint8_t y, uint32_t line){

	uint32_t *lcd=(uint32_t *)&lcd_buffer[y][x];

	uint32_t mask=0;
	uint32_t alpha=line&0x11111111;

	mask|=alpha;
	alpha<<=1;
	mask|=alpha;
	alpha<<=1;
	mask|=alpha;
	alpha<<=1;
	mask|=alpha;

	uint32_t old_line=*lcd;

	*lcd=old_line ^ ((old_line ^ line) & mask);

}

inline void lcd_draw_byte(uint8_t x,uint8_t y, uint8_t byte){
	uint8_t pixel=lcd_buffer[y][x/2];
	uint8_t mask=0;

	if (byte & 0x01){
		mask|=0x0F;
	}
	if (byte & 0x10){
		mask|=0xF0;
	}

	lcd_buffer[y][x/2]=pixel ^ ((pixel ^ byte) & mask);

}

void lcd_draw_row(uint8_t x,uint8_t y, uint8_t width, uint32_t color){

	uint8_t w=width;
	uint8_t color4=to_4bit_color(color)|0x11;
	uint32_t line=(color4<<24)|(color4<<16)|(color4<<8)|(color4);

	if (x%2==1){
		lcd_draw_pixel(x,y, color4);
		x++;
		w--;
	}

	while (w>8){
		lcd_draw_8pix(x/2,y,line);
		x+=8;
		w-=8;
	}

	while (w>1){
		lcd_draw_byte(x,y, color4);
		x+=2;
		w-=2;
	}

	if (w==1){
		lcd_draw_pixel(x,y, color4);
	}

}

void lcd_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

	for (;height>0;y++,height--){
		lcd_draw_row(x,y,width,color);
	}

}

void lcd_draw_small_num(uint8_t x,uint8_t y, uint8_t num){

	icon_t icon = {.id=0,.x=6+num,.y=0,.w=1,.h=1,.data=icons_dat};

	lcd_draw_icon(x,y,icon);
}

void lcd_draw_icon(uint8_t x,uint8_t y, icon_t icon){

//	icon_t icon=icons[icon_id];
	int start_x=icon.x*ICON_UNIT_WIDTH;
	int start_y=icon.y*ICON_DATA_WIDTH*ICON_UNIT;

	for (int h=0;h<icon.h*ICON_UNIT&&(y+h)<LCD_HEIGHT;h++){
		for (int w=0;w<icon.w&&((x/2+(w+1)*ICON_UNIT_WIDTH)<=LCD_LINE_SIZE);w++){
				uint32_t line=((uint32_t *)icon.data)[(start_y+ICON_DATA_WIDTH*h+start_x+w*ICON_UNIT_WIDTH)/4];
				lcd_draw_8pix(x/2+w*ICON_UNIT_WIDTH,y+h,line);
		}
	}


}

static ret_code_t lpm013m126a_init(void) {

	nrf_gpio_cfg_output(BACKLIGHT_PIN);
	nrf_gpio_cfg_output(LCD_CS_PIN);
	nrf_gpio_cfg_output(LCD_POWER_PIN);
	nrf_gpio_cfg_output(LCD_COM_PIN);

	nrf_gpio_pin_set(LCD_POWER_PIN);

	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG(LCD_SPI_INSTANCE);
	nrf_drv_spi_init(&spi, &spi_config, NULL);

	return NRF_SUCCESS;
}

static void lpm013m126a_uninit(void){

}

static void lpm013m126a_pixel_draw(uint16_t x, uint16_t y, uint32_t color){

	uint8_t pixel=lcd_buffer[y][x/2];
	uint8_t mask;

	if (x&1){
		mask=0x0F;
	}
	else{
		mask=0xF0;
	}

	lcd_buffer[y][x/2]= pixel ^ ((pixel ^ to_4bit_color(color)) & mask);
}

static void lpm013m126a_rect_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

//	for (int i=0;i<height;i++){
//		for(int j=0;j<width;j++){
//			lpm013m126a_pixel_draw(x+j,y+i,color);
//		}
//	}
	lcd_draw_rect(x,y,width,height,color);

}

static void lpm013m126a_display(void){

    nrf_gpio_pin_set(LCD_CS_PIN);
    nrf_drv_spi_transfer(&spi, lcd_update_command, sizeof(lcd_update_command), NULL, 0);

    for (int i=0; i<LCD_HEIGHT;i++){
        nrf_drv_spi_transfer(&spi, lcd_buffer[i], LCD_LINE_SIZE, NULL, 0);

        lcd_nextline[1]=i+2;
        nrf_drv_spi_transfer(&spi, lcd_nextline, sizeof(lcd_nextline), NULL, 0);

    }

    nrf_drv_spi_transfer(&spi, lcd_noupdate_command, sizeof(lcd_noupdate_command), NULL, 0);


    nrf_gpio_pin_clear(LCD_CS_PIN);

    lcd_nextline[1]=0;
}

static void lpm013m126a_rotation_set(nrf_lcd_rotation_t rotation){

}

static void lpm013m126a_invert(bool invert){

}

static lcd_cb_t lpm013m126a_cb = {
    .height = LPM013M126A_HEIGHT,
    .width = LPM013M126A_WIDTH
};


const nrf_lcd_t nrf_lcd_lpm013m126a = {
    .lcd_init = lpm013m126a_init,
    .lcd_uninit = lpm013m126a_uninit,
    .lcd_pixel_draw = lpm013m126a_pixel_draw,
    .lcd_rect_draw = lpm013m126a_rect_draw,
    .lcd_display = lpm013m126a_display,
    .lcd_rotation_set = lpm013m126a_rotation_set,
    .lcd_display_invert = lpm013m126a_invert,
    .p_lcd_cb = &lpm013m126a_cb
};
