#include <emusic.h>

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <glib.h>

//#include "../../ui/ui.h"
#include "callback.h"

extern Em_Smart_Data  *em;

	static gint 
_on_server_quit (xmmsv_t *val, void* data) 
{
	xmmsc_unref (em->conn);
	ERR( "Server Quit" );
	em->conn = NULL;
	return TRUE;
}

	static void 
_on_server_disconnect (void *data)
{
	ERR( "Server gone" );
}


	static int 
_on_playlist_content_changed( xmmsv_t* value, void* user_data )
{

	emusic_playlist_update(em->conn, em->main_playlist);

	return TRUE;
}


	static int 
_on_playback_status_changed( xmmsv_t *value, void *data )
{	
	Evas_Object *ic;

	//--------设定playback_status
	if ( !xmmsv_get_int(value, &(em->playback_status)) )
	{
		em->playback_status = XMMS_PLAYBACK_STATUS_STOP;
		return TRUE;
	}

	switch( em->playback_status )
	{
		case XMMS_PLAYBACK_STATUS_PLAY:
			{
				xmmsc_result_t* res2;

				//FIXME
				INF("Switch to play\n");

				break;
			}
		case XMMS_PLAYBACK_STATUS_STOP:
			INF("Switch to STOP");
			

		case XMMS_PLAYBACK_STATUS_PAUSE:
			INF("Switch to pause\n");

			break;
	}

	return TRUE;
}

	static int 
_on_playlist_loaded(xmmsv_t* value, gpointer user_data)
{
	char* name;
	if( !xmmsv_is_error(value) && xmmsv_get_string(value, (const char**)&name) )
	{

		g_free(em->main_playlist);
		em->main_playlist = g_strdup(name);

		/* invalidate currenyly played track id. */
		em->cur_track_id = 0;
		
		emusic_playlist_update(em->conn, em->main_playlist);
	}

	return TRUE;
}

	static int 
_on_playback_playtime_changed( xmmsv_t* value, void* data )
{	
	int32_t time;
	if ( xmmsv_is_error(value)
			|| ! xmmsv_get_int(value, &time))
		return TRUE;

	time /= 1000;

	//emusic_slider_update(em->slider, time, NULL);

	if( em->slider.cur_track_duration > 0 && em->slider.slider_runing == FALSE ) {
		em->slider.playtime = time;
		//INF("%f", em->slider.proportion);
		emusic_slider_update(em->slider, em->slider.cur_track_duration, em->slider.playtime);  ////////ERRS???????
		//elm_slider_value_set(em->slider.slider, proportion);
	}
	return TRUE;
}

	static int 
_on_playback_cur_track_changed( xmmsv_t* value, void* user_data )
{
	if( xmmsv_get_int(value, &(em->cur_track_id)) && (em->cur_track_id) != 0)
	{
		Emu_Media_Info *mdinfo;
		mdinfo = emusic_medialib_info_get(em->conn, em->cur_track_id);

		if (mdinfo != NULL)
		{
			em->slider.cur_track_duration = mdinfo->duration;

			emusic_cover_art_update(em->cover_art.obj, mdinfo->cover_path);
			emusic_info_update(em->info.obj, mdinfo->artist, mdinfo->album, mdinfo->title);
			emusic_slider_update(em->slider.obj, em->slider.cur_track_duration, 0.0);

			/*FIXME: free mdinfo???*/
			free(mdinfo);
		}
	}
	else
	{
		xmmsc_result_t *result;
		xmmsv_t *return_value;
		int hh;
	
		/*    get current track ID    */
		result = xmmsc_playlist_current_pos (em->conn, XMMS_ACTIVE_PLAYLIST);
		xmmsc_result_wait (result);
		return_value = xmmsc_result_get_value (result);
//		xmmsc_result_wait (return_value);
		xmmsv_get_int (return_value, hh);

		/* FIXME: Cann't get the pos id??*/
//    	xmmsc_result_notifier_set (result, bc_playlist_current_pos, NULL);
//    	xmmsc_result_unref (result);
		
	}

	return TRUE;
}

	static int 
_on_collection_changed( xmmsv_t* dict, void* user_data )
{
	/* xmmsc_result_dict_foreach(res, dict_foreach, NULL); */

	xmmsv_t *string_value, *int_value;
	const char *name, *ns;
	gint32 type;

	xmmsv_dict_get (dict, "name", &string_value);
	xmmsv_get_string( string_value, &name);

	xmmsv_dict_get (dict, "namespace", &string_value);
	xmmsv_get_string( string_value, &ns );

	xmmsv_dict_get (dict, "type", &int_value);	
	xmmsv_get_int( int_value, &type );

	/* g_debug("name=%s, ns=%s, type=%d", name, ns, type); */

	/* currently we only care about playlists */
	if( ns && strcmp(ns, "Playlists") == 0 )
	{
		switch(type)
		{
			case XMMS_COLLECTION_CHANGED_ADD:
				//            add_playlist_to_menu( name, TRUE );
				break;
			case XMMS_COLLECTION_CHANGED_UPDATE:
				break;
			case XMMS_COLLECTION_CHANGED_RENAME:
				// rename_playlist( name, newname );
				break;
			case XMMS_COLLECTION_CHANGED_REMOVE:
				//            remove_playlist_from_menu( name );
				break;
		}
	}
	return TRUE;
}

static int 
_on_media_lib_entry_changed(xmmsv_t* value, void* user_data)
{
	/* g_debug("mlib entry changed"); */
#if 0
	int32_t id = 0;
	if( xmmsv_get_int (value, &id) )
	{
		GtkTreeModel* model = (GtkTreeModel*)list_store;
		GtkTreeIter it;
		if( !model )
			return TRUE;
		/* FIXME: This is damn inefficient, but I didn't have a
		 * better way now.
		 * Maybe it can be improved using custom tree model. :-( */
		if( gtk_tree_model_get_iter_first(model, &it) )
		{
			uint32_t _id;
			do{
				gtk_tree_model_get(model, &it, COL_ID, &_id, -1);
				if( _id == id )
				{
					/* g_debug("found! update: %d", id); */
					queue_update_track( id, &it );
					break;
				}
			}while(gtk_tree_model_iter_next(model, &it));
		}
	}
#endif
	return TRUE;
}

	static void 
_config_changed_foreach(const char *key, xmmsv_t *value, void* user_data)
{
	Evas_Object *ic;

	ic = elm_icon_add(em->mediaplayer);

	if( strncmp( key, "playlist.", 9) == 0 )
	{
		const char* val;
		g_assert( xmmsv_get_string( value, &val ) );

		if( strcmp( key + 9, "repeat_one") == 0 )
		{
			if( val[0] == '1' )
			{
				em->repeat_mode = REPEAT_CURRENT;
				elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_once");
			}
			else
			{
				if( em->repeat_mode == REPEAT_CURRENT )
				{
					em->repeat_mode = REPEAT_NONE;
					elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off");
				}
			}
		}
		else if( strcmp( key + 9, "repeat_all") == 0 )
		{
			if( val[0] == '1' )
			{
				em->repeat_mode = REPEAT_ALL;
				elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_all");
			}
			else
			{
				if( em->repeat_mode == REPEAT_ALL )
				{	
					em->repeat_mode = REPEAT_NONE;
					elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off");
				}
			}
		}
		elm_button_icon_set(em->playlist_ctl.repeat_btn, ic);
	}
}

	static int 
_on_configval_changed(xmmsv_t* value, void* user_data)
{
	xmmsv_dict_foreach( value, _config_changed_foreach, NULL );
	return TRUE;
}



	int 
emusic_setup_callback(Em_Smart_Data *em)
{
	xmmsc_result_t *res;

	xmmsc_disconnect_callback_set (em->conn, _on_server_disconnect, NULL);
	/* play status */
	res = xmmsc_playback_status(em->conn);
	xmmsc_result_notifier_set_and_unref(res, _on_playback_status_changed, NULL);
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_playback_status, _on_playback_status_changed, NULL );

	/* server quit */
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_quit, _on_server_quit, NULL );

	/* playlist loaded */
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_playlist_loaded, _on_playlist_loaded, NULL );

	/* play time */
	/* this is a signal rather than broadcast, so restart is needed in the callback func. */
	XMMS_CALLBACK_SET( em->conn, xmmsc_signal_playback_playtime, _on_playback_playtime_changed, NULL);

	/* playlist changed */
	/* we have to find a way to block this when we change the playlist with the artist_show and so on in main_menu.c */
	//    XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_playlist_changed, _on_playlist_content_changed, NULL );

	/* current track info */
	res = xmmsc_playback_current_id(em->conn);
	xmmsc_result_notifier_set_and_unref( res, _on_playback_cur_track_changed, NULL );
	/* FIXME: If the playback is paused , the broadcast will not work???? */
	/* FIXME: We cann't use xmmsc_broadcast_playlist_current_pos ?????? */
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_playback_current_id, _on_playback_cur_track_changed, NULL );
	
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_playlist_current_pos, _on_playback_cur_track_changed, NULL );

	/* media lib */
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_medialib_entry_changed, _on_media_lib_entry_changed, NULL );

	//    XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_collection_changed, _on_collection_changed, NULL );

	/* config values */
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_config_value_changed, _on_configval_changed, NULL );


	/*      INIT the repeat and playlist mode       */
	emusic_playlist_sort( em->conn, em->main_playlist, NULL );
	em->shuffle_mode = XMMS_PLAYLIST_CHANGED_SORT;
	
	xmmsc_config_set_value(em->conn, "playlist.repeat_one", "0");
	xmmsc_config_set_value(em->conn, "playlist.repeat_all", "0");

	

	return 1;
}


