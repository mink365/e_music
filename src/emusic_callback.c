
#include "emusic_callback.h"
#include "emusic.h"
#include "utils.h"
#include "main_ui.h"
#include "emusic_config.h"
#include "emusic_medialib_info.h"

extern Smart_Data  *sd;

//////////////////////////////////////////////////////
	void
emusic_play(int id)
{
	printf("item ID [%d]\n", id);

	xmmsc_result_t* res;
	res = xmmsc_playlist_set_next( sd->connection, id );
	xmmsc_result_unref(res);

	res = xmmsc_playback_tickle( sd->connection );
	xmmsc_result_unref(res);

}

	static int 
_on_playlist_content_received( xmmsv_t* value, Evas_Object *playlist_show )
{

	int pl_size = xmmsv_list_get_size( value );
	int i;

	for ( i = 0; i < pl_size; i++ ) 
	{
		int32_t id;
		xmmsv_t *current_value;
		xmmsc_result_t *res;

		const char *title;
		const char *errmsg;
		xmmsv_t *dict_entry;
		xmmsv_t *infos;
		xmmsv_t *return_value;

		xmmsv_list_get( value, i, &current_value );
		xmmsv_get_int( current_value, &id );

		Emu_Media_Info *mdinfo;
		mdinfo = emusic_get_medialib_info(sd->connection, id);

		emusic_playlist_append( sd->playlist_show, mdinfo, (int *)i );
	}

	return TRUE;
}


	void 
emusic_update_play_list( Evas_Object *playlist_show )
{
	xmmsc_result_t *res;

	emusic_playlist_clear(playlist_show);

	res = xmmsc_playlist_list_entries( sd->connection, sd->cur_playlist );
	xmmsc_result_notifier_set_and_unref( res, (xmmsc_result_notifier_t)_on_playlist_content_received, playlist_show );

}

///----------------------------------------------------------------------------------

	int 
xmms_quit(xmmsv_t *value, void *data)
{
	elm_exit();
	return TRUE;
}


	static gint 
_on_server_quit (xmmsv_t *val, void* data) 
{
	xmmsc_unref (sd->connection);
	g_warning( "Server Quit" );
	sd->connection = NULL;
	xmms_quit( NULL, NULL );
	return TRUE;
}

	static void 
_on_server_disconnect (void *data)
{
	g_warning( "Server gone" );
	xmms_quit( NULL, NULL );
}


	static int 
_on_playlist_content_changed( xmmsv_t* value, void* user_data )
{

	emusic_update_play_list(sd->playlist_show);

	return TRUE;
}


	static int 
_on_playback_status_changed( xmmsv_t *value, void *data )
{	
	Evas_Object *ic;

	//--------设定playback_status
	if ( !xmmsv_get_int(value, &(sd->playback_status)) )
	{
		sd->playback_status = XMMS_PLAYBACK_STATUS_STOP;
		return TRUE;
	}

	switch( sd->playback_status )
	{
		case XMMS_PLAYBACK_STATUS_PLAY:
			{
				xmmsc_result_t* res2;

				ic = elm_icon_add(sd->mediaplayer);                                    
				elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_play"); 
				elm_button_icon_set(sd->play_btn, ic);
				INF("Switch to play\n");

				break;
			}
		case XMMS_PLAYBACK_STATUS_STOP:
			INF("Switch to STOP");
			ic = elm_icon_add(sd->mediaplayer);                                    
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_stop"); 
			elm_button_icon_set(sd->play_btn, ic);

		case XMMS_PLAYBACK_STATUS_PAUSE:
			ic = elm_icon_add(sd->mediaplayer);                                    
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_pause"); 
			elm_button_icon_set(sd->play_btn, ic);
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

		g_free(sd->cur_playlist);
		sd->cur_playlist = g_strdup(name);

		/* invalidate currenyly played track id. */
		sd->cur_track_id = 0;

		/* if there are pending requests, cancel them */
		//        cancel_pending_update_tracks();

		emusic_update_play_list( sd->playlist_show );
	}

	printf("loaded Playlist!!!\n");
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

	char time_buf[32];
	timeval_to_str( time, time_buf, G_N_ELEMENTS(time_buf) );	
	edje_object_part_text_set( elm_layout_edje_get(sd->mediaplayer), "text.playtime.label", time_buf );

	if( sd->cur_track_duration > 0 )
	{		
		//        g_signal_handlers_block_by_func(progress_bar, on_progress_bar_changed, user_data);
		//        gtk_range_set_value( (GtkRange*)progress_bar,
		//                              (((gdouble)100000 * time) / cur_track_duration) );
		//        g_signal_handlers_unblock_by_func(progress_bar, on_progress_bar_changed, user_data);
		elm_slider_value_set(sd->slider, (((gdouble)1000 * time) / sd->cur_track_duration));
	}
	return TRUE;
}


	static int 
_on_playback_cur_track_changed( xmmsv_t* value, void* user_data )
{
	if( xmmsv_get_int(value, &(sd->cur_track_id)) && (sd->cur_track_id) != 0)
	{
		Emu_Media_Info *mdinfo;
		mdinfo = emusic_get_medialib_info(sd->connection, sd->cur_track_id);

		if (mdinfo != NULL)
		{

			if (mdinfo->cover_path)
			{
				evas_object_hide(sd->cover);
				evas_object_del(sd->cover);
				sd->cover = elm_image_add(sd->mediaplayer);
				elm_image_file_set(sd->cover, mdinfo->cover_path, NULL);	
				elm_layout_content_set(sd->mediaplayer, "cover.swallow", sd->cover);
				evas_object_show(sd->cover);
			}
			else
			{
				/* Default cover ??? */
				evas_object_hide(sd->cover);
				evas_object_del(sd->cover);
				sd->cover = elm_image_add(sd->mediaplayer);
				elm_image_file_set(sd->cover, emusic_config_theme_get(), "icon/cover");	
				elm_layout_content_set(sd->mediaplayer, "cover.swallow", sd->cover);
				evas_object_show(sd->cover);
			}

		
			elm_button_label_set(sd->artist_show, mdinfo->artist);

			elm_button_label_set(sd->album_show, mdinfo->album);

			elm_button_label_set(sd->title_show, mdinfo->title);

			sd->cur_track_duration = mdinfo->duration;
			char time_buf[32];
			timeval_to_str( mdinfo->duration/1000, time_buf, G_N_ELEMENTS(time_buf) );	
			edje_object_part_text_set( elm_layout_edje_get(sd->mediaplayer), "text.duration.label", time_buf );

			/*FIXME: free mdinfo???*/
			free(mdinfo);
		}
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

	printf("collection changed \n");

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

	ic = elm_icon_add(sd->mediaplayer);

	if( strncmp( key, "playlist.", 9) == 0 )
	{
		const char* val;
		g_assert( xmmsv_get_string( value, &val ) );

		if( strcmp( key + 9, "repeat_one") == 0 )
		{
			if( val[0] == '1' )
			{
				sd->repeat_mode = REPEAT_CURRENT;
				elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_once");
			}
			else
			{
				if( sd->repeat_mode == REPEAT_CURRENT )
				{
					sd->repeat_mode = REPEAT_NONE;
					elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off");
				}
			}
		}
		else if( strcmp( key + 9, "repeat_all") == 0 )
		{
			if( val[0] == '1' )
			{
				sd->repeat_mode = REPEAT_ALL;
				elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_all");
			}
			else
			{
				if( sd->repeat_mode == REPEAT_ALL )
				{	
					sd->repeat_mode = REPEAT_NONE;
					elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_repeat_off");
				}
			}
		}
	
		elm_button_icon_set(sd->repeat_btn, ic);
	}

}

	static int 
_on_configval_changed(xmmsv_t* value, void* user_data)
{
	xmmsv_dict_foreach( value, _config_changed_foreach, NULL );
	return TRUE;
}



	int 
setup_xmms2_callback(Smart_Data *sd)
{
	xmmsc_result_t *res;

	xmmsc_disconnect_callback_set (sd->connection, _on_server_disconnect, NULL);
	/* play status */
	res = xmmsc_playback_status(sd->connection);
	xmmsc_result_notifier_set_and_unref(res, _on_playback_status_changed, NULL);
	XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_playback_status, _on_playback_status_changed, NULL );

	/* server quit */
	XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_quit, _on_server_quit, NULL );

	/* playlist loaded */
	XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_playlist_loaded, _on_playlist_loaded, NULL );

	/* play time */
	/* this is a signal rather than broadcast, so restart is needed in the callback func. */
	XMMS_CALLBACK_SET( sd->connection, xmmsc_signal_playback_playtime, _on_playback_playtime_changed, NULL);

	/* playlist changed */
	/* we have to find a way to block this when we change the playlist with the artist_show and so on in main_menu.c */
	//    XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_playlist_changed, _on_playlist_content_changed, NULL );

	/* current track info */
	res = xmmsc_playback_current_id(sd->connection);
	xmmsc_result_notifier_set_and_unref( res, _on_playback_cur_track_changed, NULL );
	/* FIXME: If the playback is paused , the broadcast will not work???? */
	XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_playback_current_id, _on_playback_cur_track_changed, NULL );

	/* media lib */
	XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_medialib_entry_changed, _on_media_lib_entry_changed, NULL );

	//    XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_collection_changed, _on_collection_changed, NULL );

	/* config values */
	XMMS_CALLBACK_SET( sd->connection, xmmsc_broadcast_config_value_changed, _on_configval_changed, NULL );


	/*      INIT the repeat and playlist mode       */
	emusic_playlist_sort( sd->connection, sd->cur_playlist, NULL );
	sd->shuffle_mode = XMMS_PLAYLIST_CHANGED_SORT;
	
	xmmsc_config_set_value(sd->connection, "playlist.repeat_one", "0");
	xmmsc_config_set_value(sd->connection, "playlist.repeat_all", "0");

	

	return 1;
}


