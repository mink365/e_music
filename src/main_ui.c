
#include <Elementary.h>

#include "main_ui.h"
#include "emusic.h"
#include "ui/ui.h"

extern Em_Smart_Data  *em;

static void
_win_del(void *data, Evas_Object *obj, void *event_info)
{
	elm_exit();
}

/*
	Win
*/
int
creat_win(Em_Smart_Data *em)
{
	Evas_Object *obj, *win;

	win = elm_win_add(NULL, "e_music", ELM_WIN_BASIC);
	em->win = win;
	em->evas = evas_object_evas_get(win);

	em->layout = elm_layout_add(win);
	if (!em->layout)
	{
		ERR("could not create layout.\n");
		evas_object_del(win);
		return 0;
	}

	evas_object_size_hint_weight_set(em->layout, 1.0, 1.0);
	elm_win_resize_object_add(win, em->layout);
	evas_object_show(em->layout);

	em->edje = elm_layout_edje_get(em->layout);
	if (!elm_layout_file_set(em->layout, emusic_config_theme_get(), "emusic/main"))
	{
		int err = edje_object_load_error_get(em->edje);
		const char *errmsg = edje_load_error_str(err);
		ERR("cannot load theme '%s', group '%s': %s\n",
				emusic_config_theme_get(), "emusic/main", errmsg);
		evas_object_del(win);
		return 0;
	}

	Evas_Object *pg = elm_pager_add(win);
	em->pager = pg;
	elm_layout_content_set(em->layout, "pager", pg);
	elm_object_style_set(pg, "slide_invisible");
	evas_object_show(pg);


	if (emusic_playlist_toolbar_creat(em)) {
		elm_pager_content_push(pg, em->playlist_view);
	}
	else {
		ERR("could not create playlist dialog.\n");
	}

	Evas_Object *notify = emusic_notify_creat(em);
	if (notify) {
		//edje_object_part_swallow(em->edje, _EDJE_PART_NOTIFY, notify);
	}
	else {
		ERR("could not create notify.\n");
	}

	if (emusic_event_bg_creat(em)) {
		edje_object_part_swallow(em->edje, "event_bg", em->event_bg);
	}
	else {
		ERR("could not create event bg.\n");
	}

	elm_win_title_set(win, "E_Music");
	evas_object_smart_callback_add(win, "delete,request", _win_del, NULL);
	evas_object_resize(win, 800, 400);
	evas_object_show(win);

	return 1;
}

/*
	Main_Menu
*/
static void
_button_clicked_setting_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data *em = data;
	
	if (emusic_config_dialog_creat(em)) {
		elm_pager_content_push(em->pager, em->config_dialog);
	}
	else
		ERR("could not create configure dialog.");
}

int 
emusic_play_menu_creat(Em_Smart_Data *em)
{
	Evas_Object *mediaplayer, *ic, *btn;
	
  	mediaplayer = edje_object_add(em->evas);
  	if (!edje_object_file_set(mediaplayer, emusic_config_theme_get(), "main/mediaplayer_view"))
  	{
    	int err = edje_object_load_error_get(mediaplayer);
    	const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(), "main/mediaplayer_view", errmsg);
    	evas_object_del(mediaplayer);
    	return 0;
  	}
  	evas_object_size_hint_weight_set(mediaplayer, 1.0, 1.0);
  	evas_object_show(mediaplayer);

  	em->mediaplayer = mediaplayer;

	/* Setting button */
	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/setting"); 
	btn = elm_button_add(mediaplayer);
	evas_object_smart_callback_add(btn, "clicked", _button_clicked_setting_cb, em);
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_button_icon_set(btn, ic);       
	elm_object_style_set(btn, "simple");
	edje_object_part_swallow(mediaplayer, "setting_bt.swallow", btn);
	evas_object_show(btn);
	evas_object_show(ic);
	
	if (emusic_playlist_ctl_creat(em)) {
		edje_object_part_swallow(mediaplayer, "playlist_ctl.swallow", em->playlist_ctl);
	}
	else {
		ERR("could not create playlist_ctl widget.\n");
	}

	if (emusic_info_creat(em)) {
		edje_object_part_swallow(mediaplayer, "songs_ifo.swallow", em->info);
	}
	else {
		ERR("could not create info widget.\n");
	}

	if (emusic_cover_art_creat(em)) {
		edje_object_part_swallow(mediaplayer, "cover_art.swallow", em->cover_art);
	}
	else {
		ERR("could not create cover_art widget.\n");
	}

	if (emusic_play_ctl_creat(em)) {
		edje_object_part_swallow(mediaplayer, "play_ctl.swallow", em->play_ctl);
	}
	else {
		ERR("could not create play_ctl widget.\n");
	}

	if (emusic_slider_creat(em)) {
		edje_object_part_swallow(mediaplayer, "slider.swallow", em->slider);
	}
	else {
		ERR("could not create slider widget.\n");
	}

	/* call the backend-callback to update information */
	emusic_callback_update(em);

	return TRUE;
}
