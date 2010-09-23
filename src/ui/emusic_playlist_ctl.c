#include <Elementary.h>
#include <emusic.h>
#include <xmmsclient/xmmsclient.h>
#include "emusic_playlist_ctl.h"

static void _button_clicked_playlist_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_repeat_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_shuffle_cb(void *data, Evas_Object *obj, void *event_info);

typedef struct _Smart_Data Smart_Data;

struct _Smart_Data
{
	Evas_Object *repeat_btn;
	Evas_Object *shuffle_btn;
};


int 
emusic_playlist_ctl_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *playlist_ctl_box, *ic, *btn;

	Smart_Data *sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return;

	obj = edje_object_add(em->evas);
	if (!edje_object_file_set(obj, emusic_config_theme_get(), "widget/playlist_ctl"))
	{
		int err = edje_object_load_error_get(obj);
		const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),"widget/playlist_ctl", errmsg);
    	evas_object_del(obj);
    	return NULL;
	}
	em->playlist_ctl.obj = obj;
	//evas_object_smart_data_set(em->playlist_ctl, sd);

	playlist_ctl_box = elm_box_add(obj);
	elm_box_homogenous_set(playlist_ctl_box, 0);
	elm_box_horizontal_set(playlist_ctl_box, 1);


	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_playlist"); 
	btn = elm_button_add(obj);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_playlist_cb, em);
	elm_button_icon_set(btn, ic);         
	elm_box_pack_end(playlist_ctl_box, btn);
	evas_object_show(btn);
	evas_object_show(ic);
	
	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off"); 
	btn = elm_button_add(obj);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_repeat_cb, em);
	elm_button_icon_set(btn, ic);
	elm_box_pack_end(playlist_ctl_box, btn);
	evas_object_show(btn);
	evas_object_show(ic);
    em->playlist_ctl.repeat_btn = btn;

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_off"); 
	btn = elm_button_add(obj);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_shuffle_cb, em);
	elm_button_icon_set(btn, ic);
	elm_box_pack_end(playlist_ctl_box, btn);
	evas_object_show(btn);
	evas_object_show(ic);
    em->playlist_ctl.shuffle_btn = btn;

	edje_object_part_swallow(obj, "box.swallow", playlist_ctl_box);
	evas_object_show(playlist_ctl_box);

	return 1;
}

	static void
_button_clicked_playlist_cb(void *data, Evas_Object *obj, void *event_info)
{
	//emusic_update_play_list(em->playlist_show);
	emusic_playlist_update(em->conn, em->main_playlist);
	emusic_switch_to_playlist();
}

	static void
_button_clicked_repeat_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	
	if (em->repeat_mode == REPEAT_ALL)	
	{
		emusic_playlist_set_repeat_all(em->conn, FALSE);
	}
	else
	{
		if (em->repeat_mode == REPEAT_CURRENT)
		{
			emusic_playlist_set_repeat_one(em->conn, FALSE);
			emusic_playlist_set_repeat_all(em->conn, TRUE);
		
		}
		else
		{
			/* now we repeat none so go repeat one */
			emusic_playlist_set_repeat_one(em->conn, TRUE);
		}
	}

}

	static void
_button_clicked_shuffle_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *ic;
	Em_Smart_Data *em = data;

	/* we have no broadcast so We have to do it our self */	
	if( em->shuffle_mode == XMMS_PLAYLIST_CHANGED_SHUFFLE ){
		emusic_playlist_sort( em->conn, NULL, NULL );

		ic = elm_icon_add(em->mediaplayer);                                    
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_off"); 
		elm_button_icon_set(obj, ic);

		em->shuffle_mode = XMMS_PLAYLIST_CHANGED_SORT;
	}
	else{
		emusic_playlist_shuffle( em->conn, NULL );

		ic = elm_icon_add(em->mediaplayer);                                    
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_on"); 
		elm_button_icon_set(obj, ic);

		em->shuffle_mode = XMMS_PLAYLIST_CHANGED_SHUFFLE;
	}
}


