/*
 * app_hrm.h
 *
 *  Created on: Sep 26, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef APP_APP_HRM_H_
#define APP_APP_HRM_H_

#include "FreeRTOS.h"
#include "arm_math.h"
#include "buttons.h"
#include "task.h"

#define HRM_BUFFER_WIDTH 200
#define HRM_GRAPH_WIDTH 160
#define SAMPLES_PER_SET 20

#define FFT_LENGTH 128

//q31_t hrm_graph_ac[HRM_GRAPH_WIDTH*2];
//q31_t hrm_graph_filtered[HRM_GRAPH_WIDTH];
//float32_t hrm_graph_unfiltered[HRM_GRAPH_WIDTH];
//float32_t hrm_graph_filtered[HRM_GRAPH_WIDTH];

extern TaskHandle_t hrm_task;

void hrm_process(void);
void hrm_draw(void);
void hrm_handle_button_evt(button_event_t* evt);
void hrm_add_samples(int32_t* samples);
void hrm_start(void);
void hrm_stop(void);
void hrm_enter(void);
void hrm_exit(void);

#endif /* APP_APP_HRM_H_ */
