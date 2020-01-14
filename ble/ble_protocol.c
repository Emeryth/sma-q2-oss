/*
 * ble_protocol.c
 *
 *  Created on: Jan 9, 2020
 *      Author: andrju
 */

#include "ble_protocol.h"
#include "ble_init.h"
#include "nordic_common.h"
#include "nrf.h"
#include "sma-q2.h"
#include "FreeRTOS.h"
#include "app_timer.h"
#include "app_time.h"


void ble_handle_message( uint8_t * p_data, uint16_t length){

	uint8_t msg_type=p_data[0];

    if (msg_type==MSG_SET_TIME){
    	uint32_t date;
    	memcpy(&date,p_data+1,4);

    	set_date(date);
    }

}
