/*
 * hrm.h
 *
 *  Created on: Oct 8, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_HRM_H_
#define HARDWARE_HRM_H_

enum hrm_commands{
	HRM_NOP,
	HRM_START,
	HRM_STOP,
};

void hrm_init(void);
void hrm_start(void);
void hrm_stop(void);
float hrm_get_hr(void);

#endif /* HARDWARE_HRM_H_ */
