/*
 * status.c
 *
 *  Created on: Oct 11, 2019
 *      Author: Andrzej Surowiec
 */

#include "status.h"


typedef struct status{

bool ble_connected;
bool ble_pairing_request;

} status_t;

status_t watch_status={.ble_connected=false,.ble_pairing_request=false};

bool status_get_ble_connected(void){
	return watch_status.ble_connected;
}

void status_set_ble_connected(bool status){
	 watch_status.ble_connected=status;
}

bool status_get_pairing_request(void){
	return watch_status.ble_pairing_request;
}

void status_set_pairing_request(bool status){
	watch_status.ble_pairing_request=status;
}
