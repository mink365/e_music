#ifndef __MEIDALIB_INFO__
#define __MEDIALIB_INFO__

Emu_Media_Info *emusic_medialib_info_get(xmmsc_connection_t *conn, int id);
int emusic_media_scan(xmmsc_connection_t *conn, char *playlist, char *file);

//<-----------
//emusic_album_cover_path

#endif
