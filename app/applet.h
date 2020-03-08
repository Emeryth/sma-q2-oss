/*
 * applet.h
 *
 *  Created on: May 31, 2019
 *      Author: Andrzej Surowiec
 */

#ifndef APP_APPLET_H_
#define APP_APPLET_H_

#include "buttons.h"


typedef struct{

	int id;
	void (*process_applet)(void);
	void (*draw_applet)(void);
	void (*handle_button_evt)(button_event_t *evt);
	void (*enter_applet)(void);
	void (*exit_applet)(void);
	int previous_applet;


}applet_t;

enum applet_id{
	APPLET_WATCHFACE,
	APPLET_MENU,
	APPLET_TETRIS,
	APPLET_HRM,
	APPLET_MUSIC,
	APPLET_BATTERY,
	APPLET_CALL,
	APPLET_WEATHER,
	APPLET_NOTIFICATIONS,
	APPLET_NOTIF_POPUP,
	NUM_APPLETS
};

extern applet_t applets[NUM_APPLETS];

#endif /* APP_APPLET_H_ */
