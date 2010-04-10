
#include <Elementary.h>

#include "main_ui.h"
#include "e_music.h"
#include "xmms2_callback.h"
#include "collections.h"
#include "playback.h"

extern Smart_Data  *sd;

void
e_music_switch_pager()
{
	printf(" switch pager!!\n");
	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
}

/*      playback events from buttons        */
static void
_button_clicked_play_cb(void *data, Evas_Object *obj, void *event_info)
{
    Smart_Data *sd = data;

	if( sd->playback_status == XMMS_PLAYBACK_STATUS_PLAY ){
		printf("Switch to ........ pause");
		playback_pause(sd->connection);
	}
	else{
		printf("Switch to ..........");
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

//	_update_info(sd);
}

/*       playlist            */
static void
_button_clicked_playlist_cb(void *data, Evas_Object *obj, void *event_info)
{
//	Pginfo *info = data;
	printf(" switch pager!!\n");
	e_music_update_play_list(sd->playlist_show);
	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
}

static void
_button_clicked_repeat_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;
}

static void
_button_clicked_shuffle_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *ic;

	/* we have no broadcast so We have to do it our self */	
	if( sd->shuffle_mode == XMMS_PLAYLIST_CHANGED_SHUFFLE ){
		e_music_playlist_sort( sd->connection, sd->cur_playlist, NULL );
		
		ic = elm_icon_add(sd->layout);                                    
		elm_icon_file_set(ic, THEME, "icon/mp_shuffle_off"); 
		elm_button_icon_set(sd->shuffle_btn, ic);

		sd->shuffle_mode = XMMS_PLAYLIST_CHANGED_SORT;
	}
	else{
		e_music_playlist_shuffle( sd->connection, sd->cur_playlist );
		
		ic = elm_icon_add(sd->layout);                                    
		elm_icon_file_set(ic, THEME, "icon/mp_shuffle_on"); 
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
    
	artist = elm_button_label_get( obj );
	
	strcat(buf, artist);
	strcat(buf, "'");
	
//	printf("%s\n", buf);
	
	collections_creat ( sd->connection, buf );

	xmmsc_playlist_clear(sd->connection, NULL);

	collections_add_to_playlist( sd->connection, NULL);

	e_music_update_play_list(sd->playlist_show);

	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);

}

static void
_button_clicked_album_cb(void *data, Evas_Object *obj, void *event_info)
{
    Smart_Data *sd = data;
    char *album;
//    char buf[32];
    char buf[PATH_MAX] = "album:'";
    
	album = elm_button_label_get( obj );
	
	strcat(buf, album);
	strcat(buf, "'");
//	printf("%s\n", buf);
	collections_creat ( sd->connection, buf );

	xmmsc_playlist_clear(sd->connection, NULL);

	collections_add_to_playlist( sd->connection, NULL);

	e_music_update_play_list(sd->playlist_show);

	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);

}

static void
_button_clicked_title_cb(void *data, Evas_Object *obj, void *event_info)
{
    Smart_Data *sd = data;
    char buf[PATH_MAX] = "title:'";
    	
	strcat(buf, "*");
	strcat(buf, "'");
	
//	printf("%s\n", buf);
	
	collections_creat ( sd->connection, buf );

	xmmsc_playlist_clear(sd->connection, NULL);

	collections_add_to_playlist( sd->connection, NULL);

	e_music_update_play_list(sd->playlist_show);

	elm_flip_go(fl, ELM_FLIP_ROTATE_Y_CENTER_AXIS);

}

/*    slider      */
static void
_slider_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	Smart_Data *sd = data;
	
	printf("Slider callback!!\n");
	xmmsc_result_t* res;
    double val = elm_slider_value_get(obj);
    uint32_t new_play_time = val * sd->cur_track_duration;
	printf("val: %f ,new play time : %d, cur duration : %d\n", val, new_play_time, sd->cur_track_duration );
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


   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);


   fl = elm_flip_add(win);
   evas_object_size_hint_align_set(fl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(fl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, fl);


	evas_object_show(fl);

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
	//Evas_Object *playlist_ctl_box; 导致段错误？？？

	Evas_Object *ic;
    Evas_Object *bt;
    
    Evas_Object *playlist_ctl_box;

	Evas_Object *ifo_box;

//-----------calloc-----
//	sd = E_MUSIC_NEW(Smart_Data, 1);

  	sd->layout = elm_layout_add(win);
  	if (!sd->layout)
  	{
    	ERR("could not create layout.\n");
    	evas_object_del(win);
    	return 0;
  	}

	elm_flip_content_front_set(fl, sd->layout);

	evas_object_size_hint_weight_set(sd->layout, 1.0, 1.0);
	elm_win_resize_object_add(win, sd->layout);
  	evas_object_show(sd->layout);


  	sd->edje = elm_layout_edje_get(sd->layout);
  	if (!elm_layout_file_set(sd->layout, THEME, _EDJE_GROUP_MAIN))
  	{
    		int err = edje_object_load_error_get(sd->edje);
    		const char *errmsg = edje_load_error_str(err);
    		ERR("cannot load theme '%s', group '%s': %s\n",
        		THEME, _EDJE_GROUP_MAIN, errmsg);
   		 	evas_object_del(win);
    		return 0;
  	}


/*             cover                             */
	sd->cover = elm_image_add(sd->layout);
	elm_image_file_set(sd->cover, THEME, "icon/cover");	
	elm_layout_content_set(sd->layout, "cover.swallow", sd->cover);
	evas_object_show(sd->cover);


/*              playlis control                          */
    playlist_ctl_box = elm_box_add(sd->layout);
    elm_box_homogenous_set(playlist_ctl_box, 0);
    elm_box_horizontal_set(playlist_ctl_box, 1);

    ELM_ADD(playlist_ctl_box, "icon/mp_playlist",  _button_clicked_playlist_cb);
//    ELM_ADD(playlist_ctl_box, "icon/mp_repeat_off",  _button_clicked_repeat_cb);
//    ELM_ADD(playlist_ctl_box, "icon/mp_shuffle_off",  _button_clicked_shuffle_cb);
    ic = elm_icon_add(sd->layout);                                    
    elm_icon_file_set(ic, THEME, "icon/mp_repeat_off"); 
	sd->repeat_btn = elm_button_add(sd->layout);
	evas_object_smart_callback_add(sd->repeat_btn, "clicked", _button_clicked_repeat_cb, sd);
	elm_button_icon_set(sd->repeat_btn, ic);         
    elm_object_style_set(sd->repeat_btn, "simple");
    elm_box_pack_end(playlist_ctl_box, sd->repeat_btn);
   	evas_object_show(sd->repeat_btn);
   	evas_object_show(ic);

    ic = elm_icon_add(sd->layout);                                    
    elm_icon_file_set(ic, THEME, "icon/mp_shuffle_off"); 
	sd->shuffle_btn = elm_button_add(sd->layout);
	evas_object_smart_callback_add(sd->shuffle_btn, "clicked", _button_clicked_shuffle_cb, sd);
	elm_button_icon_set(sd->shuffle_btn, ic);            
    elm_object_style_set(sd->shuffle_btn, "simple");
    elm_box_pack_end(playlist_ctl_box, sd->shuffle_btn);
   	evas_object_show(sd->shuffle_btn);
   	evas_object_show(ic);

	

    elm_layout_content_set(sd->layout, "playlist_ctl.swallow", playlist_ctl_box);
    evas_object_show(playlist_ctl_box);
    
    
/*
       ------------info-------------                
*/

	ifo_box = elm_box_add(sd->layout);
	
    ic = elm_icon_add(sd->layout);                                    
    elm_icon_file_set(ic, THEME, "icon/artist");
    sd->artist_show = elm_button_add(sd->layout);
	evas_object_size_hint_align_set(sd->artist_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_smart_callback_add(sd->artist_show, "clicked", _button_clicked_artist_cb, sd); 
	elm_button_icon_set(sd->artist_show, ic);
	elm_button_label_set(sd->artist_show, "Artist");            
    elm_object_style_set(sd->artist_show, "simple");
    elm_box_pack_end(ifo_box, sd->artist_show);
   	evas_object_show(sd->artist_show);
   	evas_object_show(ic);

    ic = elm_icon_add(sd->layout);                                    
    elm_icon_file_set(ic, THEME, "icon/album"); 
	sd->album_show = elm_button_add(sd->layout);
	evas_object_smart_callback_add(sd->album_show, "clicked", _button_clicked_album_cb, sd);
	evas_object_size_hint_align_set(sd->album_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_button_icon_set(sd->album_show, ic);
	elm_button_label_set(sd->album_show, "Album");            
    elm_object_style_set(sd->album_show, "simple");
    elm_box_pack_end(ifo_box, sd->album_show);
   	evas_object_show(sd->album_show);
   	evas_object_show(ic);

	ic = elm_icon_add(sd->layout);                                    
	elm_icon_file_set(ic, THEME, "icon/title"); 
	sd->title_show = elm_button_add(sd->layout);	
	evas_object_size_hint_align_set(sd->title_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(sd->title_show, "clicked", _button_clicked_title_cb, sd);
	elm_button_icon_set(sd->title_show, ic);
	elm_button_label_set(sd->title_show, "Title");            
	elm_object_style_set(sd->title_show, "simple");
	elm_box_pack_end(ifo_box, sd->title_show);
   	evas_object_show(sd->title_show);
   	evas_object_show(ic);

    elm_layout_content_set(sd->layout, "songs_ifo.swallow", ifo_box);
    evas_object_show(ifo_box);   	

         
//---------------------------------

    play_ctl_box = elm_box_add(sd->layout);
    elm_box_homogenous_set(play_ctl_box, 0);
    elm_box_horizontal_set(play_ctl_box, 1);

    ELM_ADD(play_ctl_box, "icon/mp_prev",  _button_clicked_prev_cb);

//    ELM_ADD(play_ctl_box, "icon/mp_play",  _button_clicked_play_cb);
    ic = elm_icon_add(sd->layout);                                    
    elm_icon_file_set(ic, THEME, "icon/mp_play"); 
	sd->play_btn = elm_button_add(sd->layout);
	evas_object_smart_callback_add(sd->play_btn, "clicked", _button_clicked_play_cb, sd);
	elm_button_icon_set(sd->play_btn, ic);          
    elm_object_style_set(sd->play_btn, "simple");
    elm_box_pack_end(play_ctl_box, sd->play_btn);
   	evas_object_show(sd->play_btn);
   	evas_object_show(ic);

    ELM_ADD(play_ctl_box, "icon/mp_next",  _button_clicked_next_cb);

    elm_layout_content_set(sd->layout, "play_ctl.swallow", play_ctl_box);
    evas_object_show(play_ctl_box);
///------------------------------

/*                  slider                       */
   sd->slider = elm_slider_add(sd->layout);
	evas_object_smart_callback_add(sd->slider, "delay,changed", _slider_changed_cb, sd);
   elm_slider_label_set(sd->slider, "Label");
//   elm_slider_icon_set(sl, ic);
   elm_slider_unit_format_set(sd->slider, "%1.1f units");
   elm_slider_span_size_set(sd->slider, 120);
   evas_object_size_hint_align_set(sd->slider, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sd->slider, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_content_set(sd->layout, "slider.swallow", sd->slider);
   evas_object_show(sd->slider);

   return 1;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////playlist
int creat_playlist(Smart_Data *sd)
{
	Evas_Object *tb, *lb, *list, *sc;

	tb = elm_table_add(win);	
	elm_flip_content_back_set(fl, tb);
   	evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   	evas_object_show(tb);
	
	sd->playlist_show = elm_genlist_add(win);
//	elm_list_always_select_mode_set(sd->playlist_show, 1);
	evas_object_size_hint_align_set(sd->playlist_show, EVAS_HINT_FILL, EVAS_HINT_FILL);
   	evas_object_size_hint_weight_set(sd->playlist_show, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(sd->playlist_show);	
	elm_table_pack(tb, sd->playlist_show, 1, 0, 1, 1);


//	e_music_update_play_list(sd->playlist_show);

	return 1;
}


