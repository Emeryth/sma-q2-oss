/*
 * buttons.h
 *
 *  Created on: Oct 20, 2018
 *      Author: Andrzej Surowiec
 */

#ifndef HARDWARE_BUTTONS_H_
#define HARDWARE_BUTTONS_H_

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "sma-q2.h"
#include "app_timer.h"
#include "app_button.h"
#include "nrf_drv_gpiote.h"
#include "nrf_nvic.h"
#include  "FreeRTOS.h"
#include  "queue.h"


#define BUTTON_LONG_PRESS_THRESHOLD 10

enum buttons{

	BUTTON_BACK,
	BUTTON_UP,
	BUTTON_OK,
	BUTTON_DOWN,
	BUTTON_NUM

};

enum press_types{

	SHORT_PRESS,
	SHORT_PRESS_RELEASE,
	LONG_PRESS,
	LONG_PRESS_HOLDING,
	LONG_PRESS_RELEASE

};

typedef struct{

	int button;
	int press_type;
	int hold_counter;


}button_event_t;

extern int button_pins[BUTTON_NUM];
extern QueueHandle_t button_evt_queue;

void buttons_init(void);
void send_button_evt(uint8_t button,uint8_t evt,int cnt);

#endif /* HARDWARE_BUTTONS_H_ */
