/*
 * status.c
 *
 *  Created on: Oct 11, 2019
 *      Author: Andrzej Surowiec
 */

#include "status.h"

typedef struct status{

uint8_t ble_connected;

} status_t;

status_t watch_status;

uint8_t status_get_ble_connected(void){
	return watch_status.ble_connected;
}

void status_set_ble_connected(uint8_t status){
	 watch_status.ble_connected=status;
}
