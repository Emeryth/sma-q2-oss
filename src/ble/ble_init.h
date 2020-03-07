/*
 * ble_init.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef BLE_BLE_INIT_H_
#define BLE_BLE_INIT_H_

#include <stdint.h>
#include <string.h>
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "ble_nus.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t  m_ble_stack_thread;

void gap_params_init(void);
void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);
void services_init(void);
void on_conn_params_evt(ble_conn_params_evt_t * p_evt);
void conn_params_error_handler(uint32_t nrf_error);
void conn_params_init(void);
void on_adv_evt(ble_adv_evt_t ble_adv_evt);
void on_ble_evt(ble_evt_t * p_ble_evt);
void ble_evt_dispatch(ble_evt_t * p_ble_evt);
void ble_stack_init(void);
void advertising_init(void);
void ble_stack_thread(void * arg);

#endif /* BLE_BLE_INIT_H_ */
