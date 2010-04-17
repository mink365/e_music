#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include "emusic.h"
#include <glib.h>

enum {
    REPEAT_NONE,
    REPEAT_CURRENT,
    REPEAT_ALL
};

int setup_xmms2_callback(Smart_Data *sd);

int xmms_quit(xmmsv_t *value, void *data);

void emusic_update_play_list( Evas_Object *playlist );

void emusic_play(int id);

#endif
