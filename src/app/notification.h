/*
 * notification.h
 *
 *  Created on: Feb 27, 2020
 *      Author: andrju
 */

#ifndef APP_NOTIFICATION_H_
#define APP_NOTIFICATION_H_

#include "pb_decode.h"
#include "smaq2oss.pb.h"

#define NOTIFCATION_STORAGE_MAX_SIZE 1000

typedef struct notification{

	uint16_t length;
	uint8_t *data;
	bool unread;

}notification_t;

extern MessageNotification notification_buf;

void notification_init(void);
void notification_store(uint8_t *data,uint16_t length);
int notification_get_count(void);
int notification_get_unread_count(void);
void notification_clear_unread(void);
notification_t* notification_get(uint8_t num);
int notification_decode(notification_t *notif);

#endif /* APP_NOTIFICATION_H_ */
