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
#include "weather.h"
#include "pb_decode.h"
#include "smaq2oss.pb.h"


void ble_handle_message( uint8_t * p_data, uint16_t length){

	uint8_t msg_type=p_data[0];
	int status;

    if (msg_type==MSG_SET_TIME){
    	uint32_t date;
//    	memcpy(&date,p_data+1,4);
    	SetTime set_time = SetTime_init_zero;
    	pb_istream_t stream = pb_istream_from_buffer(p_data+1, length-1);
    	status = pb_decode(&stream, SetTime_fields, &set_time);

    	if (status){
    		set_date(set_time.timestamp);
    	}


    }
    else if(msg_type==MSG_SET_WEATHER){
    	weather_set(p_data);
    }

}

void ble_send_music_event(uint8_t evt){

	uint16_t len=2;
	uint8_t data[2];

	data[0]=MSG_MUSIC_EVENT;
	data[1]=evt;

	ble_send(data,len);

}
