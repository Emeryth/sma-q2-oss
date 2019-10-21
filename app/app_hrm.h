/*
 * app_hrm.h
 *
 *  Created on: Sep 26, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef APP_APP_HRM_H_
#define APP_APP_HRM_H_

#include "buttons.h"
#include "arm_math.h"

#define HRM_BUFFER_WIDTH 200
#define HRM_GRAPH_WIDTH 160
#define SAMPLES_PER_SET 20

#define FFT_LENGTH 128

int hrm_values[HRM_BUFFER_WIDTH];
q31_t hrm_graph[HRM_GRAPH_WIDTH];
//q31_t hrm_graph_ac[HRM_GRAPH_WIDTH*2];
//q31_t hrm_graph_filtered[HRM_GRAPH_WIDTH];


int hrm_command;

void draw_hrm(void);
void hrm_handle_button_evt(button_event_t *evt);
void hrm_add_samples(int32_t *samples);
void hrm_start();
void hrm_stop();


#endif /* APP_APP_HRM_H_ */
