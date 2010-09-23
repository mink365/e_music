#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

void emusic_playback_play (xmmsc_connection_t *conn);
void emusic_playback_pause (xmmsc_connection_t *conn);
void emusic_playback_next (xmmsc_connection_t *conn);
void emusic_playback_prev (xmmsc_connection_t *conn);
int emusic_playback_init(Em_Smart_Data *sd);

void emusic_playback_jump(xmmsc_connection_t *conn, int id);
void emusic_playlist_shuffle (xmmsc_connection_t *conn, char *playlist);
void emusic_playlist_sort (xmmsc_connection_t *conn, char *playlist, char *ctx);

void emusic_playback_seek(xmmsc_connection_t *conn, uint32_t time);


#endif
