#include <emusic.h>

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <glib.h>

#include "callback.h"

extern Em_Smart_Data  *em;

static int 
_on_server_quit (xmmsv_t *val, void* data) 
{
	xmmsc_unref(em->conn);
	ERR( "Server Quit" );
	em->conn = NULL;
	return TRUE;
}

static void 
_on_server_disconnect (void *data)
{
	xmmsc_unref(em->conn);
	ERR( "Server gone" );
	em->conn = NULL;
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
	int playback_status;
	if (!xmmsv_get_int(value, &(playback_status)))
	{
		playback_status = XMMS_PLAYBACK_STATUS_STOP;
		emusic_play_ctl_update(PLAYBACK_STATUS_STOP);
		return FALSE;
	}
	INF("%d", playback_status);

	switch(playback_status)
	{
		case XMMS_PLAYBACK_STATUS_PLAY:
			emusic_play_ctl_update(PLAYBACK_STATUS_PLAY);
			break;
		case XMMS_PLAYBACK_STATUS_STOP:
			emusic_play_ctl_update(PLAYBACK_STATUS_STOP);
			break;
		case XMMS_PLAYBACK_STATUS_PAUSE:
			emusic_play_ctl_update(PLAYBACK_STATUS_PAUSE);
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
		
		emusic_playlist_update(em->main_playlist);
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

	emusic_slider_update(FALSE, time);

	return TRUE;
}

static int 
_on_playback_cur_track_changed( xmmsv_t* value, void* user_data )
{
	if( xmmsv_get_int(value, &(em->cur_track_id)) && (em->cur_track_id) != 0)
	{
		Emu_Media_Info *mdinfo;
		mdinfo = emusic_medialib_info_get(em->cur_track_id);

		if (mdinfo != NULL)
		{
			emusic_cover_art_update(mdinfo->cover_path);
			emusic_info_update(mdinfo->artist, mdinfo->album, mdinfo->title);
			emusic_slider_update(mdinfo->duration, FALSE);
		}
		free(mdinfo);
	}
	else
	{
		xmmsc_result_t *result;
		xmmsv_t *return_value;
		int pos;
	
		/*    get current track ID    */
		result = xmmsc_playlist_current_pos (em->conn, XMMS_ACTIVE_PLAYLIST);
		xmmsc_result_wait (result);
		return_value = xmmsc_result_get_value (result);
		xmmsv_get_int (return_value, pos);
		
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


	/* currently we only care about playlists */
	if( ns && strcmp(ns, "Playlists") == 0 )
	{
		switch(type)
		{
			case XMMS_COLLECTION_CHANGED_ADD:
				//   add_playlist_to_menu( name, TRUE );
				break;
			case XMMS_COLLECTION_CHANGED_UPDATE:
				break;
			case XMMS_COLLECTION_CHANGED_RENAME:
				// rename_playlist( name, newname );
				break;
			case XMMS_COLLECTION_CHANGED_REMOVE:
				//  remove_playlist_from_menu( name );
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
	INF("config changed %s", key);
	if( strncmp( key, "playlist.", 9) == 0 )
	{
		const char* val;
		g_assert( xmmsv_get_string( value, &val ) );

		if( strcmp( key + 9, "repeat_one") == 0 )
		{
			if( val[0] == '1' )
			{
				emusic_playlist_ctl_repeat(REPEAT_CURRENT);
			}
			else
			{
					emusic_playlist_ctl_repeat(REPEAT_NONE);
			}
		}
		else if( strcmp( key + 9, "repeat_all") == 0 )
		{
			if( val[0] == '1' )
			{
				emusic_playlist_ctl_repeat(REPEAT_ALL);
			}
			else
			{
				emusic_playlist_ctl_repeat(REPEAT_NONE);
			}
		}
	}
}

static int 
_on_configval_changed(xmmsv_t* value, void* user_data)
{
	xmmsv_dict_foreach( value, _config_changed_foreach, NULL );
	return TRUE;
}



int 
emusic_callback_setup(Em_Smart_Data *em)
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

	//XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_collection_changed, _on_collection_changed, NULL );

	/* config values */
	XMMS_CALLBACK_SET( em->conn, xmmsc_broadcast_config_value_changed, _on_configval_changed, NULL );


	/*      INIT the repeat and playlist mode       */
	emusic_playlist_sort( em->conn, em->main_playlist, NULL );
	
	xmmsc_config_set_value(em->conn, "playlist.repeat_one", "0");
	xmmsc_config_set_value(em->conn, "playlist.repeat_all", "0");

	return TRUE;
}

int 
emusic_callback_update(Em_Smart_Data *em)
{
	xmmsc_result_t *res;

	/* play status */
	res = xmmsc_playback_status(em->conn);
	xmmsc_result_notifier_set_and_unref(res, _on_playback_status_changed, NULL);

	/* current track info */
	res = xmmsc_playback_current_id(em->conn);
	xmmsc_result_notifier_set_and_unref( res, _on_playback_cur_track_changed, NULL );

	return TRUE;
}
