#include <Elementary.h>
#include <emusic.h>
#include "emusic_info.h"

static void _button_clicked_album_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_artist_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_title_cb(void *data, Evas_Object *obj, void *event_info);


typedef struct _Smart_Data Smart_Data;

struct _Smart_Data
{
  Evas_Object *artist_show;
  Evas_Object *album_show;
  Evas_Object *title_show;
};

int 
emusic_info_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *info_box, *ic, *btn;
	
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
	em->info.obj = obj;

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
    em->info.artist_show = btn;
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
    em->info.album_show = btn;
	evas_object_show(btn);
	evas_object_show(ic);

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/title"); 
	btn = elm_button_add(obj);	
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_title_cb, em);
	elm_button_icon_set(btn, ic);
	elm_button_label_set(btn, "Title");            
	elm_object_style_set(btn, "simple");
	elm_box_pack_end(info_box, btn);
    em->info.title_show = btn;
	evas_object_show(btn);
	evas_object_show(ic);

	edje_object_part_swallow(em->info.obj, "box.swallow", info_box);
	evas_object_show(info_box);

	return 1;

}


void emusic_info_update(Evas_Object *obj, char *artist, char *album, char *title)
{
	elm_button_label_set(em->info.artist_show, artist);
	elm_button_label_set(em->info.album_show, album);
	elm_button_label_set(em->info.title_show, title);
}


	static void
_button_clicked_title_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	char buf[PATH_MAX] = "title:'";

	strcat(buf, "*");
	strcat(buf, "'");

	emusic_collections_creat( em->conn, buf );

	emusic_mlib_browse_update_artists();

	emusic_playlist_clear(em->conn, em->coll_playlist);

	emusic_collections_add_to_playlist( em->conn, em->coll_playlist);

	emusic_playlist_update(em->conn, em->main_playlist);

	edje_object_signal_emit(em->edje, "transition,hide,mediaplayer_view", "");
	edje_object_signal_emit(em->edje, "transition,show,playlist_view", "");

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

	emusic_collections_creat ( em->conn, buf );

	emusic_playlist_clear(em->conn, em->coll_playlist);

	emusic_collections_add_to_playlist( em->conn, em->coll_playlist);

	emusic_playlist_update(em->conn, em->main_playlist);

	emusic_switch_to_playlist();

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

	emusic_collections_creat ( em->conn, buf );

	emusic_playlist_clear(em->conn, em->coll_playlist);

	emusic_collections_add_to_playlist( em->conn, em->coll_playlist);

	emusic_playlist_update(em->conn, em->main_playlist);

	emusic_switch_to_playlist();

}
