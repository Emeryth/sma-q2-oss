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
	void (*draw_applet)(void);
	void (*handle_button_evt)(button_event_t *evt);


}applet_t;

#endif /* APP_APPLET_H_ */
