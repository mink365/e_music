#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include "emusic.h"

void playback_play (xmmsc_connection_t *conn);
void playback_pause (xmmsc_connection_t *conn);
void playback_next (xmmsc_connection_t *conn);
void playback_prev (xmmsc_connection_t *conn);
int playback_init(Smart_Data *sd);

void emusic_playlist_shuffle (xmmsc_connection_t *conn, char *playlist);
void emusic_playlist_sort (xmmsc_connection_t *conn, char *playlist, char *ctx);


#endif
