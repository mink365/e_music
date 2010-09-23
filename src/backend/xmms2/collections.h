#ifndef __COLLECTIONS_H__
#define __COLLECTIONS_H__

void emusic_collections_creat (xmmsc_connection_t *conn, char *pattern);

void emusic_collections_add_to_playlist(xmmsc_connection_t *conn, char *playlist);

void emusic_mlib_browse_update_artists();

#endif
