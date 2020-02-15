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
#include "app_music.h"

MusicInfo music_info = MusicInfo_init_zero;

static void on_set_time(uint8_t * p_data, uint16_t length) {
	int status;

	SetTime set_time = SetTime_init_zero;
	pb_istream_t stream = pb_istream_from_buffer(p_data, length);
	status = pb_decode(&stream, SetTime_fields, &set_time);
	if (status) {
		set_date(set_time.timestamp);
	}
}


static void on_set_music_info(uint8_t * p_data, uint16_t length) {
	int status;

	pb_istream_t stream = pb_istream_from_buffer(p_data, length);
	status = pb_decode(&stream, MusicInfo_fields, &music_info);

	if (status) {
		music_set_music_info(&music_info);
	}
}

void ble_handle_message( uint8_t * p_data, uint16_t length){

	uint8_t msg_type=p_data[0];
	p_data++;
	length--;

	switch (msg_type) {
		case MSG_SET_TIME:
			on_set_time(p_data,length);
			break;
		case MSG_SET_WEATHER:
			weather_set(p_data);
			break;
		case MSG_SET_MUSIC_INFO:
			on_set_music_info(p_data,length);
			break;
		default:
			break;
	}

}

void ble_send_music_event(uint8_t evt){

	uint16_t len=2;
	uint8_t data[2];

	data[0]=MSG_MUSIC_EVENT;
	data[1]=evt;

	ble_send(data,len);

}
