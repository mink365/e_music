#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

void emusic_playlist_set_repeat_all(xmmsc_connection_t *conn, int i);
void emusic_playlist_set_repeat_one(xmmsc_connection_t *conn, int i);
void emusic_playlist_clear(xmmsc_connection_t *conn, char *playlist);

void emusic_playlist_update(xmmsc_connection_t *conn,  char *playlist);

//<----------------
//emusic_playlist_view_update
//emusic_get_medialib_info

#endif
