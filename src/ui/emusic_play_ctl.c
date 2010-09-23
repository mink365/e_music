#include <Elementary.h>
#include <emusic.h>
#include "emusic_play_ctl.h"

static void _button_clicked_prev_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_next_cb(void *data, Evas_Object *obj, void *event_info);
static void _button_clicked_play_cb(void *data, Evas_Object *obj, void *event_info);

int 
emusic_play_ctl_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *play_ctl_box, *ic, *bt;
	
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
	em->play_ctl.obj = obj;

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
	em->play_ctl.play_btn = elm_button_add(obj);
	evas_object_smart_callback_add(em->play_ctl.play_btn, "clicked", _button_clicked_play_cb, em);
	elm_button_icon_set(em->play_ctl.play_btn, ic);          
//	elm_object_style_set(em->play_btn, "simple");
	elm_box_pack_end(play_ctl_box, em->play_ctl.play_btn);
	evas_object_show(em->play_ctl.play_btn);
	evas_object_show(ic);

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

	edje_object_part_swallow(em->play_ctl.obj, "box.swallow", play_ctl_box);
	evas_object_show(play_ctl_box);

	return 1;
}


/*      playback events from buttons        */
	static void
_button_clicked_play_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	Evas_Object *ic;

	if( em->playback_status == XMMS_PLAYBACK_STATUS_PLAY ){
		emusic_playback_pause(em->conn);
		ic = elm_icon_add(em->mediaplayer);                                    
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_pause"); 
		elm_button_icon_set(em->play_ctl.play_btn, ic);
	}
	else{
		emusic_playback_play(em->conn);
		ic = elm_icon_add(em->mediaplayer);                                    
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_play"); 
		elm_button_icon_set(em->play_ctl.play_btn, ic);
	}
}

	static void
_button_clicked_prev_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;

	emusic_playback_prev(em->conn);
}

	static void
_button_clicked_next_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;

	emusic_playback_next(em->conn);
}
