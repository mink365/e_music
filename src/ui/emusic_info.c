#include <Elementary.h>
#include <emusic.h>
#include "emusic_info.h"

static void _button_clicked_album_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_artist_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_title_cb(void *data, Evas_Object *obj, void *event_info);

typedef struct _Smart_Data
{
  Evas_Object *artist_show;
  Evas_Object *album_show;
  Evas_Object *title_show;
} Smart_Data;
static Smart_Data *sd;

int 
emusic_info_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *info_box, *ic, *btn;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;
	
	obj = edje_object_add(em->evas);
	if (!edje_object_file_set(obj, emusic_config_theme_get(), "widget/info"))
	{
		int err = edje_object_load_error_get(obj);
		const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),"widget/info", errmsg);
    	evas_object_del(obj);
    	return NULL;
	}
	em->info = obj;

	info_box = elm_box_add(obj);

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/artist");
	btn = elm_button_add(obj);
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_artist_cb, em);
	elm_button_icon_set(btn, ic);
	elm_button_label_set(btn, "Artist");
	elm_object_style_set(btn, "simple");
	elm_box_pack_end(info_box, btn);
    sd->artist_show = btn;
	evas_object_show(btn);
	evas_object_show(ic);

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/album");
	btn = elm_button_add(obj);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_album_cb, em);
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_button_icon_set(btn, ic);
	elm_button_label_set(btn, "Album");            
	elm_object_style_set(btn, "simple");
	elm_box_pack_end(info_box, btn);
    sd->album_show = btn;
	evas_object_show(btn);
	evas_object_show(ic);

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/songs");
	btn = elm_button_add(obj);	
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_title_cb, em);
	elm_button_icon_set(btn, ic);
	elm_button_label_set(btn, "Title");            
	elm_object_style_set(btn, "simple");
	elm_box_pack_end(info_box, btn);
    sd->title_show = btn;
	evas_object_show(btn);
	evas_object_show(ic);

	edje_object_part_swallow(em->info, "box.swallow", info_box);
	evas_object_show(info_box);

	return 1;

}


void emusic_info_update(char *artist, char *album, char *title)
{
	if (!sd)
		return;

	elm_button_label_set(sd->artist_show, artist);
	elm_button_label_set(sd->album_show, album);
	elm_button_label_set(sd->title_show, title);
}


	static void
_button_clicked_title_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	char buf[PATH_MAX] = "title:'";

	strcat(buf, "*");
	strcat(buf, "'");

	emusic_collections_creat(buf );

	emusic_playlist_clear(em->coll_playlist);

	emusic_collections_add_to_playlist(em->coll_playlist);

	emusic_playlist_update(em->main_playlist);

	emusic_playlist_view_new(_("All songs"));

}

	static void
_button_clicked_artist_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	char *artist;
	char buf[PATH_MAX] = "artist:'";

	artist = (char *)elm_button_label_get( obj );

	strcat(buf, artist);
	strcat(buf, "'");

	emusic_collections_creat (buf );

	emusic_playlist_clear(em->coll_playlist);

	emusic_collections_add_to_playlist(em->coll_playlist);

	emusic_playlist_update(em->main_playlist);

	emusic_playlist_view_new (artist);

}

	static void
_button_clicked_album_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	char *album;
	char buf[PATH_MAX] = "album:'";

	album = (char *)elm_button_label_get( obj );

	strcat(buf, album);
	strcat(buf, "'");

	emusic_collections_creat (buf );

	emusic_playlist_clear(em->coll_playlist);

	emusic_collections_add_to_playlist(em->coll_playlist);

	emusic_playlist_update(em->main_playlist);

	emusic_playlist_view_new (album);

}
