/*
 * ble_watch_service.h
 *
 *  Created on: Feb 7, 2020
 *      Author: andrju
 */

#ifndef BLE_BLE_WATCH_SERVICE_H_
#define BLE_BLE_WATCH_SERVICE_H_

#include "ble.h"
#include "ble_srv_common.h"
#include <stdbool.h>
#include <stdint.h>

#define BLE_UUID_WATCH_SERVICE 0x0001

typedef struct ble_watchs_s ble_watchs_t;

struct ble_watchs_s {
    uint8_t uuid_type; /**< UUID type for Nordic UART Service Base UUID. */
    uint16_t service_handle; /**< Handle of Nordic UART Service (as provided by the SoftDevice). */
    ble_gatts_char_handles_t tx_handles; /**< Handles related to the TX characteristic (as provided by the SoftDevice). */
    ble_gatts_char_handles_t rx_handles; /**< Handles related to the RX characteristic (as provided by the SoftDevice). */
    uint16_t conn_handle; /**< Handle of the current connection (as provided by the SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
};

uint32_t ble_watchs_init(ble_watchs_t* p_watchs);
void ble_watchs_on_ble_evt(ble_watchs_t* p_watchs, ble_evt_t* p_ble_evt);
uint32_t ble_watchs_send(ble_watchs_t* p_watchs, uint8_t* p_data, uint16_t length);

#endif /* BLE_BLE_WATCH_SERVICE_H_ */
