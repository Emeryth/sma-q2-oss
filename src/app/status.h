/*
 * status.h
 *
 *  Created on: Oct 11, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef APP_STATUS_H_
#define APP_STATUS_H_

#include <stdint.h>
#include <stdbool.h>

bool status_get_ble_connected(void);
void status_set_ble_connected(bool status);
bool status_get_pairing_request(void);
void status_set_pairing_request(bool status);



#endif /* APP_STATUS_H_ */
