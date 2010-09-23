#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

int emusic_playlist_creat(Em_Smart_Data *em);

void emusic_playlist_view_update(Evas_Object *obj, Eina_List *list);
void emusic_playlist_view_append(Evas_Object *playlist_show, Emu_Media_Info *mdinfo, int list_id);
void emusic_playlist_view_clear(Evas_Object *obj);

#endif
