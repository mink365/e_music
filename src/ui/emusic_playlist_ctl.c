#include <Elementary.h>
#include <emusic.h>
#include <xmmsclient/xmmsclient.h>
#include "emusic_playlist_ctl.h"

typedef struct _Smart_Data
{
	Evas_Object *repeat_btn;
	Evas_Object *shuffle_btn;

	int repeat_mode;
	int shuffle_mode;
} Smart_Data;
static Smart_Data *sd;

static void _button_clicked_playlist_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_repeat_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_shuffle_cb(void *data, Evas_Object *obj, void *event_info);

int 
emusic_playlist_ctl_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *box, *ic, *bt;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;

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
	em->playlist_ctl = obj;

	box = elm_box_add(obj);
	elm_box_homogenous_set(box, 0);
	elm_box_horizontal_set(box, 1);

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_playlist");
	bt = elm_button_add(obj);
	evas_object_smart_callback_add(bt, "clicked", _button_clicked_playlist_cb, em);
	elm_button_icon_set(bt, ic);         
	elm_box_pack_end(box, bt);
	evas_object_show(bt);
	evas_object_show(ic);
	
	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off"); 
	bt = elm_button_add(obj);
	evas_object_smart_callback_add(bt, "clicked", _button_clicked_repeat_cb, em);
	elm_button_icon_set(bt, ic);
	elm_box_pack_end(box, bt);
	evas_object_show(bt);
	evas_object_show(ic);
    sd->repeat_btn = bt;

	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_off"); 
	bt = elm_button_add(obj);
	evas_object_smart_callback_add(bt, "clicked", _button_clicked_shuffle_cb, em);
	elm_button_icon_set(bt, ic);
	elm_box_pack_end(box, bt);
	evas_object_show(bt);
	evas_object_show(ic);
    sd->shuffle_btn = bt;

	edje_object_part_swallow(obj, "box.swallow", box);
	evas_object_show(box);

	return TRUE;
}

int
emusic_playlist_ctl_repeat(int mode)
{
	Evas_Object *ic;
	ic = elm_button_icon_get(sd->repeat_btn);
	sd->repeat_mode = mode;
	switch(mode)
	{
		case REPEAT_ALL:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_all");
		break;
		case REPEAT_NONE:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off");
		break;
		case REPEAT_CURRENT:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_once");
		break;
		default:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off");
		break;
	}
	return TRUE;
}

	static void
_button_clicked_playlist_cb(void *data, Evas_Object *obj, void *event_info)
{
	emusic_playlist_update(em->main_playlist);
	emusic_playlist_view_new(_("Now playing"));
}

	static void
_button_clicked_repeat_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	
	if (sd->repeat_mode == REPEAT_ALL)	
	{
		emusic_playlist_set_repeat_all(FALSE);
	}
	else
	{
		if (sd->repeat_mode == REPEAT_CURRENT)
		{
			emusic_playlist_set_repeat_one(FALSE);
			emusic_playlist_set_repeat_all(TRUE);
		
		}
		else
		{
			/* now we repeat none so go repeat one */
			emusic_playlist_set_repeat_one(TRUE);
		}
	}

}

	static void
_button_clicked_shuffle_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *ic;
	Em_Smart_Data *em = data;

	ic = elm_button_icon_get(sd->shuffle_btn);
	/* we have no broadcast so We have to do it our self */	
	if( sd->shuffle_mode == PLAYLIST_CHANGED_SHUFFLE ){
		emusic_playlist_sort( NULL, NULL );

		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_off"); 

		sd->shuffle_mode = PLAYLIST_CHANGED_SORT;
	}
	else{
		emusic_playlist_shuffle( NULL );

		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_on"); 

		sd->shuffle_mode = PLAYLIST_CHANGED_SHUFFLE;
	}
}


