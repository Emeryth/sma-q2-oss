/*
 * app_music.h
 *
 *  Created on: Jan 9, 2020
 *      Author: andrju
 */

#ifndef APP_APP_MUSIC_H_
#define APP_APP_MUSIC_H_

#include "buttons.h"

enum music_events {
    EVT_PLAY_PAUSE,
    EVT_FWD,
    EVT_REV,
    EVT_VOL_UP,
    EVT_VOL_DOWN
};

#define MUSIC_ARTIST_MAX_LEN 32
#define MUSIC_ALBUM_MAX_LEN 32
#define MUSIC_TRACK_MAX_LEN 64

typedef struct music_info {

    char artist[MUSIC_ARTIST_MAX_LEN];
    char album[MUSIC_ALBUM_MAX_LEN];
    char track[MUSIC_TRACK_MAX_LEN];

} music_info_t;

void music_process(void);
void music_draw(void);
void music_handle_button_evt(button_event_t* evt);
void music_set_music_info(music_info_t* info);

#endif /* APP_APP_MUSIC_H_ */
