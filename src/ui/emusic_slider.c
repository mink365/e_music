#include <Elementary.h>
#include <emusic.h>
#include <glib.h>

#include "emusic_slider.h"
#include "utils.h"

static void _slider_changed_cb(void *data, Evas_Object *obj, void *event_info);

int 
emusic_slider_creat(Em_Smart_Data *em)
{
	Evas_Object *slider, *obj;
	
	obj = edje_object_add(em->evas);
	if (!edje_object_file_set(obj, emusic_config_theme_get(), "widget/slider"))
	{
		int err = edje_object_load_error_get(obj);
		const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),"widget/slider", errmsg);
    	evas_object_del(obj);
    	return NULL;
	}
	em->slider.obj = obj;

	slider = elm_slider_add(obj);
	em->slider.slider = slider;
	evas_object_smart_callback_add(slider, "changed", _slider_changed_cb, em);
	elm_slider_label_set(slider, "Label");
	elm_slider_unit_format_set(slider, "%1.1f units");
	elm_slider_span_size_set(slider, 120);
	evas_object_size_hint_align_set(slider, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(slider, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	edje_object_part_swallow(obj, "slider.swallow", slider);
	evas_object_show(slider);

	return 1;
}

void 
emusic_slider_update(Evas_Object *obj, int duration, int playtime)
{
	//INF("Update slider! %d  %f", duration, prop);
	//FIXME:can't use passed duration and prop????
	if (duration) {
		char time_buf[32];
		utils_timeval_to_str( em->slider.cur_track_duration/1000, time_buf, G_N_ELEMENTS(time_buf) );	
		edje_object_part_text_set( em->mediaplayer, "text.duration.label", time_buf );
	}
	if (playtime) {
		char time_buf[32];
		utils_timeval_to_str( em->slider.playtime, time_buf, G_N_ELEMENTS(time_buf) );
		edje_object_part_text_set( em->mediaplayer, "text.playtime.label", time_buf );
		elm_slider_value_set(em->slider.slider, (((gdouble)1000 * em->slider.playtime) / em->slider.cur_track_duration));
	}
}

	static void
_slider_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;

	em->slider.slider_runing = TRUE;
	double val = elm_slider_value_get(obj);
	uint32_t new_play_time = val * em->slider.cur_track_duration;
	INF("val: %f ,new play time : %d, cur duration : %d\n", val, new_play_time, em->slider.cur_track_duration );

	emusic_playback_seek(em->conn, new_play_time);

	em->slider.slider_runing = FALSE;
}
