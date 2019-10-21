/*
 * hrm.c
 *
 *  Created on: Oct 8, 2019
 *      Author: Andrzej Surowiec
 */

#include "hrm.h"
#include "sma-q2.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_nvic.h"
#include "pah8002.h"

float hr;

void hrm_init(void){

}

void hrm_start(void){

	nrf_gpio_pin_set(HRM_POWER_PIN);
	vTaskDelay(500);

	nrf_drv_gpiote_in_event_enable(HRM_INT_PIN, true);
	pah8002_enter_normal_mode();

}

void hrm_stop(void){
	nrf_drv_gpiote_in_event_disable(HRM_INT_PIN);
	pah8002_shutdown();
}

float hrm_get_hr(void){
	return hr;
}
