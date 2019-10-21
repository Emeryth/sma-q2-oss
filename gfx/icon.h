/*
 * icon.h
 *
 *  Created on: Oct 10, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef GFX_ICON_H_
#define GFX_ICON_H_

#include <stdint.h>

#define ICON_UNIT 8
#define ICON_UNIT_WIDTH 4
#define ICON_DATA_WIDTH (128/2)
#define ICON_DATA_HEIGHT 128

typedef struct icon{
 int id;
 // Position in the sprite sheet in 8px units
 uint8_t x;
 uint8_t y;
 // Size in 8px units
 uint8_t w;
 uint8_t h;

}icon_t;

extern const unsigned char icons_dat[ICON_DATA_WIDTH*ICON_DATA_HEIGHT];

enum icons{
	ICON_HEART_BIG,
	ICON_HEART_SMALL,
	ICON_BLUETOOTH,
	ICON_BATTERY,
	ICON_CHARGE,
	NUM_ICONS
};

extern icon_t icons[NUM_ICONS];

#endif /* GFX_ICON_H_ */
