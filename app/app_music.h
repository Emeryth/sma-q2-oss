/*
 * app_music.h
 *
 *  Created on: Jan 9, 2020
 *      Author: andrju
 */

#ifndef APP_APP_MUSIC_H_
#define APP_APP_MUSIC_H_

#include "buttons.h"

void music_process(void);
void music_draw(void);
void music_handle_button_evt(button_event_t *evt);

enum music_events{
	EVT_PLAY_PAUSE,
	EVT_FWD,
	EVT_REV,
	EVT_VOL_UP,
	EVT_VOL_DOWN
};



#endif /* APP_APP_MUSIC_H_ */
