/*
 * accel.h
 *
 *  Created on: Jun 12, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_ACCEL_H_
#define HARDWARE_ACCEL_H_

#define KX022_ADDR 0x1F

void accel_init(void);
int accel_check(void);

#endif /* HARDWARE_ACCEL_H_ */
