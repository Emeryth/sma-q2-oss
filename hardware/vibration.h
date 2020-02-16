/*
 * vibration.h
 *
 *  Created on: Jun 1, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_VIBRATION_H_
#define HARDWARE_VIBRATION_H_

#include <stdint.h>

#define VIBRATION_PATTERN_LEN 8

typedef struct{

	uint8_t pattern[VIBRATION_PATTERN_LEN];
	int repeat;

}vibration_pattern_t;

typedef struct{

	vibration_pattern_t pattern;
	int step;
	int repeat;

}vibration_state_t;

void vibration_init(void);
void vibration_step(void);
void vibration_set_pattern(vibration_pattern_t pattern);
void vibration_short(void);
void vibration_stop(void);
void vibration_notification(void);
void vibration_call(void);

#endif /* HARDWARE_VIBRATION_H_ */
