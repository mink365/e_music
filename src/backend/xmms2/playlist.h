#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

void emusic_playlist_set_repeat_all(int i);
void emusic_playlist_set_repeat_one(int i);

void emusic_playlist_shuffle (char *playlist);
void emusic_playlist_sort (char *playlist, char *ctx);

void emusic_playlist_clear(char *playlist);
void emusic_playlist_update(char *playlist);

Eina_List *emusic_playlist_list_playlist();
Eina_List *emusic_playlist_list_album();
Eina_List *emusic_playlist_list_artist();
Eina_List *emusic_playlist_list_title();

//<----------------
//emusic_playlist_view_update
//emusic_get_medialib_info

#endif
