#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include "e_music.h"

void playback_play (xmmsc_connection_t *conn);
void playback_pause (xmmsc_connection_t *conn);
void playback_next (xmmsc_connection_t *conn);
void playback_prev (xmmsc_connection_t *conn);
int playback_init(Smart_Data *sd);

void e_music_playlist_shuffle (xmmsc_connection_t *conn, char *playlist);
void e_music_playlist_sort (xmmsc_connection_t *conn, char *playlist, char *ctx);


#endif
