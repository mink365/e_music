#include <Elementary.h>
#include <emusic.h>
#include <glib.h>

#include "emusic_slider.h"
#include "utils.h"

typedef struct _Smart_Data
{
  	Evas_Object *slider;

	int cur_track_duration;
	int slider_runing;
} Smart_Data;
static Smart_Data *sd;

static void _slider_changed_cb(void *data, Evas_Object *obj, void *event_info);

int 
emusic_slider_creat(Em_Smart_Data *em)
{
	Evas_Object *slider, *obj;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;

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
	em->slider = obj;

	slider = elm_slider_add(obj);
	evas_object_smart_callback_add(slider, "changed", _slider_changed_cb, em);
	elm_slider_label_set(slider, "Label");
	elm_slider_unit_format_set(slider, "%1.1f units");
	elm_slider_span_size_set(slider, 120);
	evas_object_size_hint_align_set(slider, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(slider, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	edje_object_part_swallow(obj, "slider.swallow", slider);
	evas_object_show(slider);
	sd->slider = slider;

	return 1;
}

void 
emusic_slider_update(int duration, int playtime)
{
	if (!sd)
		return;

	if (duration) {
		char time_buf[32];

		sd->cur_track_duration = duration;
		utils_timeval_to_str(duration/1000, time_buf, sizeof(time_buf) );
		edje_object_part_text_set(em->mediaplayer, "text.duration.label", time_buf );
	}
	if (playtime) {
		char time_buf[32];

		utils_timeval_to_str(playtime, time_buf, sizeof(time_buf) );
		edje_object_part_text_set( em->mediaplayer, "text.playtime.label", time_buf );
		if (sd->slider_runing != TRUE)
			elm_slider_value_set(sd->slider, (((gdouble)1000 * playtime) / sd->cur_track_duration));
	}
}

	static void
_slider_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	sd->slider_runing = TRUE;
	double val = elm_slider_value_get(obj);
	uint32_t new_play_time = val * sd->cur_track_duration;
	INF("val: %f ,new play time : %d, cur duration : %d\n", val, new_play_time, sd->cur_track_duration );

	emusic_playback_seek(new_play_time);

	sd->slider_runing = FALSE;
}
