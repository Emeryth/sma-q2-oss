/*
 * app_call.h
 *
 *  Created on: Feb 16, 2020
 *      Author: andrju
 */

#ifndef APP_APP_CALL_H_
#define APP_APP_CALL_H_

#include "applet.h"
#include "screen_mgr.h"
#include "smaq2oss.pb.h"

#define CALL_NAME_MAX_LEN 32
#define CALL_NUMBER_MAX_LEN 16

enum call_status {
    CALL_IDLE = 0,
    CALL_UNDEFINED = 1,
    CALL_ACCEPT = 1,
    CALL_INCOMING = 2,
    CALL_OUTGOING = 3,
    CALL_REJECT = 4,
    CALL_START = 5,
    CALL_END = 6,
};

typedef struct call_info {
    int command;
    char name[CALL_NAME_MAX_LEN];
    char number[CALL_NUMBER_MAX_LEN];
} call_info_t;

void call_process(void);
void call_draw(void);
void call_handle_button_evt(button_event_t* evt);
void call_enter(void);
void call_exit(void);
void call_set_info(CallNotification* notif);

int call_get_status(void);

#endif /* APP_APP_CALL_H_ */
