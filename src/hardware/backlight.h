/*
 * backlight.h
 *
 *  Created on: May 12, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_BACKLIGHT_H_
#define HARDWARE_BACKLIGHT_H_

// Backlight intensity in PWM percent
#define BACKLIGHT_INTENSITY 50
// Default backlight on time in ms
#define BACKLIGHT_TIME 3000

void backlight_init(void);
void backlight_on(void);

#endif /* HARDWARE_BACKLIGHT_H_ */
