#include <Elementary.h>
#include <emusic.h>
#include "emusic_play_ctl.h"

static void _button_clicked_prev_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_next_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_play_cb(void *data, Evas_Object *obj, void *event_info);

typedef struct _Smart_Data
{
	Evas_Object *play_btn;

	int playback_status;
} Smart_Data;
static Smart_Data *sd;

int 
emusic_play_ctl_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *play_ctl_box, *ic, *bt;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;
	
	obj = edje_object_add(em->evas);
	if (!edje_object_file_set(obj, emusic_config_theme_get(), "widget/play_ctl"))
	{
		int err = edje_object_load_error_get(obj);
		const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),"widget/play_ctl", errmsg);
    	evas_object_del(obj);
    	return NULL;
	}
	em->play_ctl = obj;

	play_ctl_box = elm_box_add(obj);
	elm_box_homogenous_set(play_ctl_box, 0);
	elm_box_horizontal_set(play_ctl_box, 1);

    ic = elm_icon_add(obj);
    elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_prev");
    elm_icon_scale_set(ic, 0, 0);
    bt = elm_button_add(obj);
    evas_object_smart_callback_add(bt, "clicked", _button_clicked_prev_cb, em);
    elm_button_icon_set(bt, ic);
    evas_object_size_hint_weight_set(bt, 0.0, 1.0);
    evas_object_size_hint_align_set(bt, 0.5, 0.5);
    elm_box_pack_end(play_ctl_box, bt);
    evas_object_show(bt);
    evas_object_show(ic);


	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_play"); 
	bt = elm_button_add(obj);
	evas_object_smart_callback_add(bt, "clicked", _button_clicked_play_cb, em);
	elm_button_icon_set(bt, ic);
	elm_box_pack_end(play_ctl_box, bt);
	evas_object_show(bt);
	evas_object_show(ic);
	sd->play_btn = bt;

    ic = elm_icon_add(obj);
    elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_next");
    elm_icon_scale_set(ic, 0, 0);
    bt = elm_button_add(obj);
    evas_object_smart_callback_add(bt, "clicked", _button_clicked_next_cb, em);
    elm_button_icon_set(bt, ic);
    evas_object_size_hint_weight_set(bt, 0.0, 1.0);
    evas_object_size_hint_align_set(bt, 0.5, 0.5);
    elm_box_pack_end(play_ctl_box, bt);
    evas_object_show(bt);
    evas_object_show(ic);

	edje_object_part_swallow(obj, "box.swallow", play_ctl_box);
	evas_object_show(play_ctl_box);

	return 1;
}

int
emusic_play_ctl_update(int mode)
{
	Evas_Object *ic;
	if (sd)
		ic = elm_button_icon_get(sd->play_btn);
	else
		return FALSE;
	sd->playback_status = mode;
	INF("%d", mode);
	switch(mode)
	{
		case PLAYBACK_STATUS_PLAY:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_play");
			break;
		case PLAYBACK_STATUS_PAUSE:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_pause");
			break;
		case PLAYBACK_STATUS_STOP:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_pause");
			break;
		default:
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_stop");
			break;
	}
	return TRUE;
}

/*      playback events from buttons        */
	static void
_button_clicked_play_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	Evas_Object *ic;

	if( sd->playback_status == PLAYBACK_STATUS_PLAY ){
		emusic_playback_pause();
	}
	else{
		emusic_playback_play();
	}
}

	static void
_button_clicked_prev_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;

	emusic_playback_prev();
}

	static void
_button_clicked_next_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;

	emusic_playback_next();
}
