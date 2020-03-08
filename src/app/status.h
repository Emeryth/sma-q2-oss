/*
 * status.h
 *
 *  Created on: Oct 11, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef APP_STATUS_H_
#define APP_STATUS_H_

#include <stdint.h>

uint8_t status_get_ble_connected(void);
void status_set_ble_connected(uint8_t status);

#endif /* APP_STATUS_H_ */
