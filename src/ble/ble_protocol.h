/*
 * ble_protocol.h
 *
 *  Created on: Jan 9, 2020
 *      Author: andrju
 */

#ifndef BLE_BLE_PROTOCOL_H_
#define BLE_BLE_PROTOCOL_H_

#include "app_music.h"
#include <stdint.h>

enum ble_message_type {
    MSG_SET_TIME = 0x01,
    MSG_BATTERY_STATE,
    MSG_MUSIC_EVENT,
    MSG_SET_WEATHER,
    MSG_SET_MUSIC_INFO,
    MSG_CALL_NOTIFICATION,
    MSG_CALL_COMMAND,
    MSG_NOTIFICATION,

};

void ble_handle_message(uint8_t* p_data, uint16_t length);
void ble_send(uint8_t* data, uint16_t length);
void ble_send_music_event(uint8_t evt);
void ble_send_call_command(uint8_t command);

#endif /* BLE_BLE_PROTOCOL_H_ */
