#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include "e_music.h"
#include <glib.h>

enum {
    REPEAT_NONE,
    REPEAT_CURRENT,
    REPEAT_ALL
};

typedef struct _TrackProperties{
    const char *artist;
    const char *album;
    const char *title;
    const char *url;
    const char *mime;
    const char *comment;
    const char *picture_front;
    int32_t duration;
    int32_t isvbr;
    int32_t bitrate;
    int32_t size;
    int32_t id;
}TrackProperties;


int setup_xmms2_callback(Smart_Data *sd);

int xmms_quit(xmmsv_t *value, void *data);

void e_music_update_play_list( Evas_Object *playlist );

void e_music_play(int id);

gboolean	get_track_properties 	(xmmsv_t *value, TrackProperties *properties);

#endif
