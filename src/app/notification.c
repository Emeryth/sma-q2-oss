/*
 * notification.c
 *
 *  Created on: Feb 27, 2020
 *      Author: andrju
 */
#include "notification.h"
#include "FreeRTOS.h"
#include "list.h"
#include "semphr.h"

MessageNotification notification_buf;

List_t notification_list;

int notification_total_size = 0;
int unread_count = 0;

SemaphoreHandle_t notif_list_mutex;

static void notification_update_unread_count(void) {

	int count = 0;

	ListItem_t *item;
	ListItem_t const *end;
	notification_t *notif;

	end = listGET_END_MARKER(&notification_list);
	item = listGET_HEAD_ENTRY(&notification_list);

	while (item != end) {

		notif = (notification_t*) listGET_LIST_ITEM_VALUE(item);

		if (notif->unread) {
			count++;
		}

		item = listGET_NEXT(item);
	}

	unread_count = count;
}

static void notification_remove(ListItem_t *notification_item) {

	notification_t *notif = (notification_t*) listGET_LIST_ITEM_VALUE(
			notification_item);

	notification_total_size -= notif->length;
	vPortFree(notif->data);
	vPortFree(notif);

	uxListRemove(notification_item);
	vPortFree(notification_item);

}

void notification_init(void) {

	vListInitialise(&notification_list);
	notif_list_mutex = xSemaphoreCreateMutex();

}

void notification_store(uint8_t *data, uint16_t length) {

	if ( xSemaphoreTake( notif_list_mutex, portMAX_DELAY) == pdTRUE) {

		while (notification_total_size + length > NOTIFCATION_STORAGE_MAX_SIZE) {
			notification_remove(listGET_HEAD_ENTRY(&notification_list));
		}

		ListItem_t *notification_item = pvPortMalloc(sizeof(ListItem_t));
		vListInitialiseItem(notification_item);

		notification_t *notification = pvPortMalloc(sizeof(notification_t));
		notification->unread = true;

		notification->data = pvPortMalloc(length);
		notification->length = length;
		memcpy(notification->data, data, length);

		notification_total_size += length;

		notification_item->xItemValue = (uint32_t) notification;
		vListInsertEnd(&notification_list, notification_item);

//	unread_count++;
		notification_update_unread_count();

		xSemaphoreGive(notif_list_mutex);
	}
}

int notification_get_count(void) {
	return listCURRENT_LIST_LENGTH(&notification_list);
}

int notification_get_unread_count(void) {
	return unread_count;
}

void notification_clear_unread(void){

	ListItem_t *item;
	ListItem_t const *end;
	notification_t *notif;

	end = listGET_END_MARKER(&notification_list);
	item = listGET_HEAD_ENTRY(&notification_list);

	while (item != end) {

		notif = (notification_t*) listGET_LIST_ITEM_VALUE(item);

		notif->unread=0;

		item = listGET_NEXT(item);
	}

	unread_count = 0;

}

notification_t* notification_get(uint8_t num) {
	if ( xSemaphoreTake( notif_list_mutex, portMAX_DELAY) == pdTRUE) {

		if (num >= notification_get_count()) {
			xSemaphoreGive(notif_list_mutex);
			return NULL;
		}

		ListItem_t *item;
		ListItem_t const *end;
		notification_t *notif;

		end = listGET_END_MARKER(&notification_list);
		item = listGET_HEAD_ENTRY(&notification_list);

		for (int i = 0; i < num; i++) {
			item=listGET_NEXT(item);
		}

		notif = (notification_t*) listGET_LIST_ITEM_VALUE(item);
		xSemaphoreGive(notif_list_mutex);
		return notif;
	}

	return NULL;
}

int notification_decode(notification_t *notif) {

	int status;
	pb_istream_t stream = pb_istream_from_buffer(notif->data, notif->length);
	status = pb_decode(&stream, MessageNotification_fields, &notification_buf);

	return status;
}
