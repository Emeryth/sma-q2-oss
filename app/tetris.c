/*
 * tetris.c
 *
 *  Created on: May 31, 2019
 *      Author: Mateusz Zalega
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "nrf_gfx.h"
#include "tetris.h"

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;

#define TETRIS_BLOCK_WIDTH 6
#define TETRIS_DRAW_X 58
#define TETRIS_DRAW_Y 32

#define  swidth 10
#define  sheight 22

uint8_t blocks[sheight][swidth];
uint8_t screen[sheight][swidth];

#define alwidth swidth * 10
#define alheight sheight * 10

#define figs 5
#define figh 3
#define figw 3
#define figr  4
uint8_t figures[figs][figr][figh][figw] = { { { { 0, 0, 0 }, { 0, 1, 1 }, { 0,
		1, 1 } }, { { 0, 0, 0 }, { 0, 1, 1 }, { 0, 1, 1 } }, { { 0, 0, 0 }, { 0,
		1, 1 }, { 0, 1, 1 } }, { { 0, 0, 0 }, { 0, 1, 1 }, { 0, 1, 1 } } }, { {
		{ 0, 0, 0 }, { 2, 2, 0 }, { 0, 2, 2 } }, { { 0, 0, 2 }, { 0, 2, 2 }, {
		0, 2, 0 } }, { { 0, 0, 0 }, { 2, 2, 0 }, { 0, 2, 2 } }, { { 0, 0, 2 }, {
		0, 2, 2 }, { 0, 2, 0 } } }, { { { 0, 0, 3 }, { 0, 0, 3 }, { 0, 3, 3 } },
		{ { 0, 0, 0 }, { 3, 0, 0 }, { 3, 3, 3 } }, { { 3, 3, 0 }, { 3, 0, 0 }, {
				3, 0, 0 } }, { { 3, 3, 3 }, { 0, 0, 3 }, { 0, 0, 0 } } }, { { {
		0, 0, 4 }, { 0, 0, 4 }, { 0, 0, 4 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 4,
		4, 4 } }, { { 0, 0, 4 }, { 0, 0, 4 }, { 0, 0, 4 } }, { { 0, 0, 0 }, { 0,
		0, 0 }, { 4, 4, 4 } } }, { { { 0, 0, 0 }, { 0, 6, 0 }, { 6, 6, 6 } }, {
		{ 0, 0, 6 }, { 0, 6, 6 }, { 0, 0, 6 } }, { { 6, 6, 6 }, { 0, 6, 0 }, {
		0, 0, 0 } }, { { 6, 0, 0 }, { 6, 6, 0 }, { 6, 0, 0 } } } };

uint8_t curfig;
uint8_t curfigx;
uint8_t curfigy;
uint8_t curfigr;

uint8_t input;

int framen;
int advance_every_f = 5;
int last_d;
int last_r;

void drawscreen() {
	memcpy((void *) (screen), (const void *) (blocks), sizeof(screen));
	for (int x_ = 0; x_ < 3; x_++) {
		for (int y_ = 0; y_ < 3; y_++) {
//            assert(curfigy + y_ < sheight);
			if (figures[curfig][curfigr][y_][x_])
				screen[curfigy + y_][curfigx + x_] =
						figures[curfig][curfigr][y_][x_];
		}
	}
}

void stampblock() {
	for (int x_ = 0; x_ < 3; x_++) {
		for (int y_ = 0; y_ < 3; y_++) {
//            assert(curfigy + y_ < sheight);
			if (figures[curfig][curfigr][y_][x_])
				blocks[curfigy + y_][curfigx + x_] =
						figures[curfig][curfigr][y_][x_];
		}
	}
}

void genblock() {
	curfig = lrand48() % figs;
	curfigx = lrand48() % (swidth - figw);
	curfigy = 0;
}

int blockhit(int yoffset) {
	for (int x_ = 0; x_ < 3; x_++) {
		for (int y_ = 0; y_ < 3; y_++) {
			if (curfigy + y_ + yoffset >= sheight)
				return 1;
			if (blocks[curfigy + y_ + yoffset][curfigx + x_]
					&& figures[curfig][curfigr][y_][x_])
				return 1;
		}
	}
	return 0;
}

void advance() {
	curfigy++;
}

void advance_till_hit() {
	while (curfigy + figh < sheight && !blockhit(1))
		advance();
	stampblock();
	genblock();
}

void rotate() {
	curfigr = (curfigr + 1) % figr;
}

void setinput(uint8_t in) {
	input = in;
}

void processinput() {
	switch (input) {
	case 'l':
		if (curfigx - 1 >= 0)
			curfigx--;
		break;
	case 'r':
		if (curfigx + figw < swidth)
			curfigx++;
		break;
	case 't':
		if (framen - last_r > advance_every_f) {
			last_r = framen;
			rotate();
		}
		break;
	case 'd':
		if (framen - last_d > advance_every_f) {
			last_d = framen;
			advance_till_hit();
		}
		break;
	}

	input = 0;
}

int testrow(int y) {
	for (int x = 1; x < swidth; x++) {
		if (!screen[y][x])
			return 0;
	}
	return 1;
}

void remove_row(int y) {
	for (; y; y--) {
		for (int x = 0; x < swidth; x++)
			blocks[y][x] = blocks[y - 1][x];
	}
	for (int x = 0; x < swidth; x++)
		blocks[0][x] = 0;
}

void processrows() {
	for (int y = sheight - 1; y; y--) {
		if (testrow(y))
			remove_row(y);
	}
}

void clear() {
	memset((void *) (blocks), 0, sizeof(blocks));
	memset((void *) (screen), 0, sizeof(screen));
}

void nextframe() {
	if (curfigy + figh >= sheight || blockhit(1)) {
		stampblock();
		genblock();
		if (blockhit(0)) {
			clear();
			genblock();
		}
	}

	processrows();
	processinput();
	if (!(framen % advance_every_f))
		advance();

	drawscreen();

	framen++;
}

void draw_tetris(void) {
	lcd_clear(BLACK);

	nextframe();

//	lcd_clear(BLACK);

	nrf_gfx_rect_t frame =
					NRF_GFX_RECT(TETRIS_DRAW_X-1, TETRIS_DRAW_Y-1, TETRIS_BLOCK_WIDTH*swidth+2, TETRIS_BLOCK_WIDTH*sheight+2);
	nrf_gfx_rect_draw(p_lcd, &frame, 1, PINK, 0);

	nrf_gfx_rect_t rect =
					NRF_GFX_RECT(TETRIS_DRAW_X, TETRIS_DRAW_Y, TETRIS_BLOCK_WIDTH, TETRIS_BLOCK_WIDTH);

	for (int i = 0; i < sheight; i++) {
		rect.x = TETRIS_DRAW_X;
		for (int j = 0; j < swidth; j++) {
			if (screen[i][j]) {
				nrf_gfx_rect_draw(p_lcd, &rect, 0, screen[i][j], 1);
			} else {

			}
			rect.x += TETRIS_BLOCK_WIDTH;
		}
		rect.y += TETRIS_BLOCK_WIDTH;
	}

//	nrf_gfx_display(p_lcd);
}

void tetris_handle_button_evt(button_event_t *evt) {

	if (evt->press_type == SHORT_PRESS) {

		switch (evt->button) {

		case BUTTON_BACK:
			setinput('l');
			break;
		case BUTTON_DOWN:
			setinput('r');
			break;
		case BUTTON_OK:
			setinput('d');
			break;
		case BUTTON_UP:
			setinput('t');
			break;

		}

	}
}

