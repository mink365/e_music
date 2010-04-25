
#include <Elementary.h>

#include "main_ui.h"
#include "emusic.h"
#include "emusic_callback.h"
#include "collections.h"
#include "playback.h"
#include "emusic_config.h"

extern Smart_Data  *sd;

static const char _EDJE_GROUP_MAIN[] = "main";
static const char _EDJE_GROUP_MEDIAPLAYER[] = "main/mediaplayer_view";
static const char _EDJE_GROUP_PLAYLIST[] = "main/playlist_view";

static const char _EDJE_PART_MEDIAPLAYER[] = "mediaplayer_view";
static const char _EDJE_PART_PLAYLIST[] = "playlist_view";


	void
emusic_switch_to_mediaplayer()
{
	edje_object_signal_emit(elm_layout_edje_get(sd->layout), "transition,hide,playlist_view", "");
	edje_object_signal_emit(elm_layout_edje_get(sd->layout), "transition,show,mediaplayer_view", "");
}

/*      playback events from buttons        */
	static void
_button_clicked_play_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;

	if( sd->playback_status == XMMS_PLAYBACK_STATUS_PLAY ){
		playback_pause(sd->connection);
	}
	else{
		playback_play(sd->connection);
	}
}

	static void
_button_clicked_prev_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;

	playback_prev(sd->connection);
}

	static void
_button_clicked_next_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;

	playback_next(sd->connection);
}

/*       playlist            */
	static void
_button_clicked_playlist_cb(void *data, Evas_Object *obj, void *event_info)
{
	emusic_update_play_list(sd->playlist_show);
	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
}

	static void
_button_clicked_repeat_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;
	
	if (sd->repeat_mode == REPEAT_ALL)	
	{
		xmmsc_config_set_value(sd->connection, "playlist.repeat_all", "0");
	}
	else
	{
		if (sd->repeat_mode == REPEAT_CURRENT)
		{
			xmmsc_config_set_value(sd->connection, "playlist.repeat_one", "0");
			xmmsc_config_set_value(sd->connection, "playlist.repeat_all", "1");
		
		}
		else
		{
			/* now we repeat none so go repeat one */
			xmmsc_config_set_value(sd->connection, "playlist.repeat_one", "1");
		}
	}

}

	static void
_button_clicked_shuffle_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *ic;

	/* we have no broadcast so We have to do it our self */	
	if( sd->shuffle_mode == XMMS_PLAYLIST_CHANGED_SHUFFLE ){
		emusic_playlist_sort( sd->connection, sd->cur_playlist, NULL );

		ic = elm_icon_add(sd->mediaplayer);                                    
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_off"); 
		elm_button_icon_set(sd->shuffle_btn, ic);

		sd->shuffle_mode = XMMS_PLAYLIST_CHANGED_SORT;
	}
	else{
		emusic_playlist_shuffle( sd->connection, sd->cur_playlist );

		ic = elm_icon_add(sd->mediaplayer);                                    
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_on"); 
		elm_button_icon_set(sd->shuffle_btn, ic);

		sd->shuffle_mode = XMMS_PLAYLIST_CHANGED_SHUFFLE;
	}
}

/*                   infos  callback                           */
	static void
_button_clicked_artist_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;
	char *artist;
	char buf[PATH_MAX] = "artist:'";

	artist = (char *)elm_button_label_get( obj );

	strcat(buf, artist);
	strcat(buf, "'");

	collections_creat ( sd->connection, buf );

	xmmsc_playlist_clear(sd->connection, NULL);

	collections_add_to_playlist( sd->connection, NULL);

	emusic_update_play_list(sd->playlist_show);

	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);

}

	static void
_button_clicked_album_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;
	char *album;
	char buf[PATH_MAX] = "album:'";

	album = (char *)elm_button_label_get( obj );

	strcat(buf, album);
	strcat(buf, "'");

	collections_creat ( sd->connection, buf );

	xmmsc_playlist_clear(sd->connection, NULL);

	collections_add_to_playlist( sd->connection, NULL);

	emusic_update_play_list(sd->playlist_show);

	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);

}

	static void
_button_clicked_title_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;
	char buf[PATH_MAX] = "title:'";

	strcat(buf, "*");
	strcat(buf, "'");


	collections_creat ( sd->connection, buf );

	xmmsc_playlist_clear(sd->connection, NULL);

	collections_add_to_playlist( sd->connection, NULL);

	emusic_update_play_list(sd->playlist_show);

	edje_object_signal_emit(elm_layout_edje_get(sd->layout), "transition,hide,mediaplayer_view", "");
	edje_object_signal_emit(elm_layout_edje_get(sd->layout), "transition,show,playlist_view", "");

}

/*    slider      */
	static void
_slider_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;

	xmmsc_result_t* res;
	double val = elm_slider_value_get(obj);
	uint32_t new_play_time = val * sd->cur_track_duration;
	INF("val: %f ,new play time : %d, cur duration : %d\n", val, new_play_time, sd->cur_track_duration );
	res = xmmsc_playback_seek_ms( sd->connection, new_play_time, XMMS_PLAYBACK_SEEK_SET );
	xmmsc_result_unref(res);
}


//-------------------------------------------------------------------------------
	static void
_win_del(void *data, Evas_Object *obj, void *event_info)
{
	elm_exit();
}



	int
creat_win(Smart_Data *sd)
{

	Evas_Object *bg, *bx;

	// set custom elementary theme
	elm_theme_overlay_add("e_music");


	win = elm_win_add(NULL, "e_music", ELM_WIN_BASIC);


	sd->layout = elm_layout_add(win);
	if (!sd->layout)
	{
		ERR("could not create layout.\n");
		evas_object_del(win);
		return 0;
	}

	evas_object_size_hint_weight_set(sd->layout, 1.0, 1.0);
	elm_win_resize_object_add(win, sd->layout);
	evas_object_show(sd->layout);

	sd->edje = elm_layout_edje_get(sd->layout);
	if (!elm_layout_file_set(sd->layout, emusic_config_theme_get(), _EDJE_GROUP_MAIN))
	{
		int err = edje_object_load_error_get(sd->edje);
		const char *errmsg = edje_load_error_str(err);
		ERR("cannot load theme '%s', group '%s': %s\n",
				emusic_config_theme_get(), _EDJE_GROUP_MAIN, errmsg);
		evas_object_del(win);
		return 0;
	}

	elm_win_title_set(win, "E_Music");
	evas_object_smart_callback_add(win, "delete,request", _win_del, NULL);
	evas_object_resize(win, 800, 480);
	evas_object_show(win);

	return 1;
}



	int 
creat_main_menu(Smart_Data *sd)
{
	//   	Evas_Object *win;
	Evas_Object *edje;
	Evas_Object *play_ctl_box;

	Evas_Object *ic;
	Evas_Object *bt;

	Evas_Object *playlist_ctl_box;

	Evas_Object *ifo_box;

	Evas_Object *mediaplayer, *ed;

	mediaplayer = elm_layout_add(win);	
  	if (!mediaplayer)
  	{
    	ERR("could not create mediaplayer widget.\n");
    	return NULL;
  	}

  	ed = elm_layout_edje_get(mediaplayer);
  	if (!elm_layout_file_set(mediaplayer, emusic_config_theme_get(), _EDJE_GROUP_MEDIAPLAYER))
  	{
    	int err = edje_object_load_error_get(ed);
    	const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(), _EDJE_GROUP_MEDIAPLAYER, errmsg);
    	evas_object_del(mediaplayer);
    	return NULL;
  	}
//  	evas_object_size_hint_weight_set(mediaplayer, 1.0, 1.0);
//	elm_win_resize_object_add(win, mediaplayer);
  	evas_object_show(mediaplayer);

  	sd->mediaplayer = mediaplayer;
  	elm_layout_content_set(sd->layout, _EDJE_PART_MEDIAPLAYER, mediaplayer);
	
	/*             cover                             */
#if 0
	sd->cover = elm_image_add(sd->layout);
	elm_image_file_set(sd->cover, emusic_config_theme_get(), "icon/cover");	
	elm_layout_content_set(sd->layout, "cover.swallow", sd->cover);
	evas_object_show(sd->cover);
#endif

	/*              playlis control                          */
	playlist_ctl_box = elm_box_add(mediaplayer);
	elm_box_homogenous_set(playlist_ctl_box, 0);
	elm_box_horizontal_set(playlist_ctl_box, 1);

//	ELM_ADD(playlist_ctl_box, "icon/mp_playlist",  _button_clicked_playlist_cb);
	//    ELM_ADD(playlist_ctl_box, "icon/mp_repeat_off",  _button_clicked_repeat_cb);
	//    ELM_ADD(playlist_ctl_box, "icon/mp_shuffle_off",  _button_clicked_shuffle_cb);
	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off"); 
	sd->repeat_btn = elm_button_add(mediaplayer);
	evas_object_smart_callback_add(sd->repeat_btn, "clicked", _button_clicked_repeat_cb, sd);
	elm_button_icon_set(sd->repeat_btn, ic);         
	elm_object_style_set(sd->repeat_btn, "simple");
	elm_box_pack_end(playlist_ctl_box, sd->repeat_btn);
	evas_object_show(sd->repeat_btn);
	evas_object_show(ic);

	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_shuffle_off"); 
	sd->shuffle_btn = elm_button_add(mediaplayer);
	evas_object_smart_callback_add(sd->shuffle_btn, "clicked", _button_clicked_shuffle_cb, sd);
	elm_button_icon_set(sd->shuffle_btn, ic);            
	elm_object_style_set(sd->shuffle_btn, "simple");
	elm_box_pack_end(playlist_ctl_box, sd->shuffle_btn);
	evas_object_show(sd->shuffle_btn);
	evas_object_show(ic);


	elm_layout_content_set(mediaplayer, "playlist_ctl.swallow", playlist_ctl_box);
	evas_object_show(playlist_ctl_box);


	/*
	   ------------info-------------                
	 */

	ifo_box = elm_box_add(mediaplayer);

	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/artist");
	sd->artist_show = elm_button_add(mediaplayer);
	evas_object_size_hint_align_set(sd->artist_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(sd->artist_show, "clicked", _button_clicked_artist_cb, sd); 
	elm_button_icon_set(sd->artist_show, ic);
	elm_button_label_set(sd->artist_show, "Artist");            
	elm_object_style_set(sd->artist_show, "simple");
	elm_box_pack_end(ifo_box, sd->artist_show);
	evas_object_show(sd->artist_show);
	evas_object_show(ic);

	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/album"); 
	sd->album_show = elm_button_add(mediaplayer);
	evas_object_smart_callback_add(sd->album_show, "clicked", _button_clicked_album_cb, sd);
	evas_object_size_hint_align_set(sd->album_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_button_icon_set(sd->album_show, ic);
	elm_button_label_set(sd->album_show, "Album");            
	elm_object_style_set(sd->album_show, "simple");
	elm_box_pack_end(ifo_box, sd->album_show);
	evas_object_show(sd->album_show);
	evas_object_show(ic);

	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/title"); 
	sd->title_show = elm_button_add(mediaplayer);	
	evas_object_size_hint_align_set(sd->title_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(sd->title_show, "clicked", _button_clicked_title_cb, sd);
	elm_button_icon_set(sd->title_show, ic);
	elm_button_label_set(sd->title_show, "Title");            
	elm_object_style_set(sd->title_show, "simple");
	elm_box_pack_end(ifo_box, sd->title_show);
	evas_object_show(sd->title_show);
	evas_object_show(ic);

	elm_layout_content_set(mediaplayer, "songs_ifo.swallow", ifo_box);
	evas_object_show(ifo_box);   	


	//---------------------------------

	play_ctl_box = elm_box_add(mediaplayer);
	elm_box_homogenous_set(play_ctl_box, 0);
	elm_box_horizontal_set(play_ctl_box, 1);

	ELM_ADD(play_ctl_box, "icon/mp_prev",  _button_clicked_prev_cb);

	//    ELM_ADD(play_ctl_box, "icon/mp_play",  _button_clicked_play_cb);
	ic = elm_icon_add(mediaplayer);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_play"); 
	sd->play_btn = elm_button_add(mediaplayer);
	evas_object_smart_callback_add(sd->play_btn, "clicked", _button_clicked_play_cb, sd);
	elm_button_icon_set(sd->play_btn, ic);          
	elm_object_style_set(sd->play_btn, "simple");
	elm_box_pack_end(play_ctl_box, sd->play_btn);
	evas_object_show(sd->play_btn);
	evas_object_show(ic);

	ELM_ADD(play_ctl_box, "icon/mp_next",  _button_clicked_next_cb);

	elm_layout_content_set(mediaplayer, "play_ctl.swallow", play_ctl_box);
	evas_object_show(play_ctl_box);
	///------------------------------

	/*                  slider                       */
	sd->slider = elm_slider_add(mediaplayer);
	evas_object_smart_callback_add(sd->slider, "delay,changed", _slider_changed_cb, sd);
	elm_slider_label_set(sd->slider, "Label");
	//   elm_slider_icon_set(sl, ic);
	elm_slider_unit_format_set(sd->slider, "%1.1f units");
	elm_slider_span_size_set(sd->slider, 120);
	evas_object_size_hint_align_set(sd->slider, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(sd->slider, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_content_set(mediaplayer, "slider.swallow", sd->slider);
	evas_object_show(sd->slider);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////playlist
int creat_playlist(Smart_Data *sd)
{
	Evas_Object *tb;

	Evas_Object *playlist, *ed;
	
	playlist = elm_layout_add(win);	
  	if (!playlist)
  	{
    	ERR("could not create playlist widget.\n");
    	return NULL;
  	}

  	ed = elm_layout_edje_get(playlist);
  	if (!elm_layout_file_set(playlist, emusic_config_theme_get(), _EDJE_GROUP_PLAYLIST))
  	{
    	int err = edje_object_load_error_get(ed);
    	const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(), _EDJE_GROUP_PLAYLIST, errmsg);
    	evas_object_del(playlist);
    	return NULL;
  	}
  	evas_object_show(playlist);

  	sd->playlist = playlist;
  	elm_layout_content_set(sd->layout, _EDJE_PART_PLAYLIST, playlist);

	tb = elm_table_add(playlist);	
	evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_content_set(playlist, "playlist_view.swallow", tb);
	evas_object_show(tb);

	sd->playlist_show = elm_genlist_add(playlist);
	evas_object_size_hint_align_set(sd->playlist_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(sd->playlist_show, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(sd->playlist_show);	
	elm_table_pack(tb, sd->playlist_show, 1, 0, 1, 1);


//	emusic_update_play_list(sd->playlist_show);

	return 1;
}
