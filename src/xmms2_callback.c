
#include "xmms2_callback.h"
#include "e_music.h"
#include "utils.h"
#include "main_ui.h"

extern Smart_Data  *sd;
/////////
//sd->cur_track_duration = 198745;



/*  repeat mod  callback  */
static int 
_on_cfg_repeat_all_received(xmmsv_t* value, void* user_data)
{
    char* val;
 	Smart_Data *sd = user_data;
	Evas_Object *ic;

    if( xmmsv_get_string(value, (const char**)&val) )
    {
        if( val && val[0] == '1' )
        {
            sd->repeat_mode = REPEAT_ALL;

            ic = elm_icon_add(sd->layout);                                    
		    elm_icon_file_set(ic, THEME, "icon/mp_repeat_all"); 
			elm_button_icon_set(sd->repeat_btn, ic);
			printf("Switch to repeat all\n");
        }
    }
    return TRUE;
}

static int 
_on_cfg_repeat_one_received(xmmsv_t* value, void* user_data)
{
    Smart_Data *sd = user_data;
    char* val;
	Evas_Object *ic;

    if( xmmsv_get_string( value, (const char**)&val) )
    {
        if( val && val[0] == '1' )
        {
            sd->repeat_mode = REPEAT_CURRENT;

            ic = elm_icon_add(sd->layout);                                    
		    elm_icon_file_set(ic, THEME, "icon/mp_repeat_once"); 
			elm_button_icon_set(sd->repeat_btn, ic);
			printf("Switch to repeat current\n");
        }
    }
    return TRUE;
}

static int 
_on_cfg_shuffle_received(xmmsv_t* value, void* user_data)
{
    Smart_Data *sd = user_data;
    char* val;
	Evas_Object *ic;

    if( xmmsv_get_string( value, (const char**)&val) )
    {
        if( val && val[0] == '1' )
        {
            sd->repeat_mode = 1;

            ic = elm_icon_add(sd->layout);                                    
		    elm_icon_file_set(ic, THEME, "icon/mp_shuffle_on"); 
			elm_button_icon_set(sd->shuffle_btn, ic);
			printf("Switch to shuffle\n");
        }
    }
    return TRUE;
}

static int 
_on_cfg_sort_received(xmmsv_t* value, void* user_data)
{
    Smart_Data *sd = user_data;
    char* val;
	Evas_Object *ic;
	printf("Switch to sort..................\n");

    if( xmmsv_get_string( value, (const char**)&val) )
    {
        if( val && val[0] == '1' )
        {
            sd->shuffle_mode = 0;

            ic = elm_icon_add(sd->layout);                                    
		    elm_icon_file_set(ic, THEME, "icon/mp_shuffle_off"); 
			elm_button_icon_set(sd->shuffle_btn, ic);
			printf("Switch to sort\n");
        }
    }
    return TRUE;
}


//////////////////////////////////////////////////////
void
e_music_play(int id)
{
   	printf("item ID [%d]\n", id);

	xmmsc_result_t* res;
	res = xmmsc_playlist_set_next( sd->connection, id );
    xmmsc_result_unref(res);

    res = xmmsc_playback_tickle( sd->connection );
    xmmsc_result_unref(res);

}
////-------------------------------------------------

static int
_update_track( xmmsv_t* value, void* user_data )
{
    	TrackProperties track_properties;
	int id = ( int )user_data;
	const char* err;
	const char *guessed_title;

	if (xmmsv_get_error (value, &err)) {
		g_warning( "Server error: %s", err );
		return TRUE;
	}

	if (!get_track_properties( value, &track_properties))
	{
//		track_properties.title = guessed_title = guess_title_from_url( track_properties.url );
	}
	e_music_playlist_append(sd->playlist_show, &track_properties, id);

	return TRUE; 
}

static int 
_on_playlist_content_received( xmmsv_t* value, Evas_Object *playlist_show )
{

	int pl_size = xmmsv_list_get_size( value );
	int i;
	Evas_Object *ic, *ic2;
    
	printf(" Go into playlist....\n");

	printf(" current_id: %d\n", sd->cur_track_id);

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

		res = xmmsc_medialib_get_info( sd->connection, id );
        	xmmsc_result_notifier_set_full( res, (xmmsc_result_notifier_t)_update_track, (int *)i, NULL );	  	
		xmmsc_result_unref( res );
    	}

	return TRUE;
}


void 
e_music_update_play_list( Evas_Object *playlist_show )
{
    xmmsc_result_t *res;

	//clear the genlist,so we just creat a new one ...
	elm_genlist_clear(playlist_show);

    	res = xmmsc_playlist_list_entries( sd->connection, sd->cur_playlist );
    	xmmsc_result_notifier_set_and_unref( res, (xmmsc_result_notifier_t)_on_playlist_content_received, playlist_show );

}



gboolean 
get_track_properties (xmmsv_t *value, TrackProperties *properties)  
{
    /* traverse the dict of dict */
    xmmsv_dict_iter_t *parent_it;

    /* fallback for title: online streams often provide channel */
    const char* channel = NULL;

    /* default values: empty */
    bzero( properties, sizeof(TrackProperties) );
    
    xmmsv_get_dict_iter (value, &parent_it);
    while (xmmsv_dict_iter_valid (parent_it ) ) 
    {
		const char *key;
		const char **val_str = NULL;
		int32_t *val_int = NULL;
		xmmsv_t *child_value;
		xmmsv_dict_iter_t *child_it;
	
		/* get child dict */
		xmmsv_dict_iter_pair (parent_it, &key, &child_value);

		/* check type of property */
		if (strcmp( key, "artist" ) == 0)
		    val_str = &(properties->artist);
		else if (strcmp( key, "album" ) == 0)
		    val_str = &(properties->album);
		else if (strcmp( key, "mime" ) == 0)
		    val_str = &(properties->mime);
		else if (strcmp( key, "comment" ) == 0)
		    val_str = &(properties->comment);
		else if (strcmp( key, "picture_front" ) == 0)
		    val_str = &(properties->picture_front);
		else if (strcmp( key, "channel" ) == 0)
		    val_str = &channel;	    	    
		else if (strcmp( key, "url" ) == 0)
		    val_str = &(properties->url);	    
		else if (strcmp( key, "title" ) == 0) 
		    val_str = &(properties->title);	    
		else if (strcmp( key, "duration" ) == 0)
		    val_int = &(properties->duration);
		else if (strcmp( key, "isvbr" ) == 0)
		    val_int = &(properties->isvbr);
		else if (strcmp( key, "bitrate" ) == 0)
		    val_int = &(properties->bitrate);
		else if (strcmp( key, "size" ) == 0)
		    val_int = &(properties->size);
		
		if (xmmsv_get_dict_iter (child_value, &child_it) && 
		    xmmsv_dict_iter_valid (child_it) && (val_int || val_str) && 
		    xmmsv_dict_iter_pair (child_it, NULL, &child_value)) 
		{	
		    if (val_int != NULL) 
			xmmsv_get_int( child_value, val_int);
		    else 
			xmmsv_get_string( child_value, val_str );    
		}
		xmmsv_dict_iter_next (parent_it);
    }

    if ((properties->title == NULL) || g_str_equal( properties->title, "" )) 
    {
		if (channel == NULL)
		    return FALSE;
		else
		    properties->title = channel;
    }
    return TRUE;
}

///----------------------------------------------------------------------------------

int 
xmms_quit(xmmsv_t *value, void *data)
{
    elm_exit();
    return TRUE;
}


static int 
_on_playlist_content_changed( xmmsv_t* value, void* user_data )
{

//	e_music_update_play_list(sd->playlist_show);

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

		    ic = elm_icon_add(sd->layout);                                    
		    elm_icon_file_set(ic, THEME, "icon/mp_play"); 
			elm_button_icon_set(sd->play_btn, ic);
			printf("Switch to play\n");
#if 0
            gtk_widget_set_tooltip_text( play_btn, _("Pause") );
            img = gtk_bin_get_child( (GtkBin*)play_btn );
            gtk_image_set_from_stock( (GtkImage*)img, GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON );
            /* FIXME: this can cause some problems sometimes... */
            res2 = xmmsc_playlist_current_pos(sd->connection, cur_playlist);
            /* mark currently played track */
#endif
//            xmmsc_result_notifier_set_and_unref(res2, on_playlist_pos_changed, NULL);
            break;
        }
        case XMMS_PLAYBACK_STATUS_STOP:
				printf("Switch to STOP");
		    	ic = elm_icon_add(sd->layout);                                    
		    	elm_icon_file_set(ic, THEME, "icon/mp_stop"); 
				elm_button_icon_set(sd->play_btn, ic);
//            gtk_label_set_text( GTK_LABEL(time_label), "--:--" );
//            gtk_range_set_value( GTK_RANGE(progress_bar), 0.0 );
//			gtk_window_set_title ((GtkWindow*)main_win, "LXMusic");
        case XMMS_PLAYBACK_STATUS_PAUSE:
		    	ic = elm_icon_add(sd->layout);                                    
		    	elm_icon_file_set(ic, THEME, "icon/mp_pause"); 
				elm_button_icon_set(sd->play_btn, ic);
				printf("Switch to pause\n");
//            gtk_widget_set_tooltip_text( play_btn, _("Play") );
//            img = gtk_bin_get_child( (GtkBin*)play_btn );
//            gtk_image_set_from_stock( (GtkImage*)img, GTK_STOCK_MEDIA_PLAY,
//                                      GTK_ICON_SIZE_BUTTON );
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
        /* FIXME: is this possible? */
//        if( cur_playlist && 0 == strcmp((char*)name, cur_playlist) )
//            return TRUE;

        g_free(sd->cur_playlist);
        sd->cur_playlist = g_strdup(name);

	/* invalidate currenyly played track id. */
        sd->cur_track_id = 0;

        /* if there are pending requests, cancel them */
//        cancel_pending_update_tracks();

//        e_music_update_play_list( sd->playlist_show );
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
	edje_object_part_text_set( sd->edje, "text.playtime.label", time_buf );

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
_on_cover_retrieve( xmmsv_t *value, void *user_data )
{
	char buf[PATH_MAX];
	unsigned int bl;
	const unsigned char *bc;
// cover 存储在内存？？？GTK用pixbuf取出一份...... QPixmap
	if( xmmsv_get_bin( value, &bc, &bl ) )
	{
		printf("BIN:%x\n", bc);	
	
	}

	return TRUE;
}
static int 
_on_playback_track_loaded( xmmsv_t* value, void* user_data )
{
    TrackProperties track_properties;
    const char* err;
    const char *guessed_title;
    
    if (xmmsv_get_error (value, &err)) {
	g_warning( "Server error: %s", err );
	return TRUE;
    }

    if (!get_track_properties( value, &track_properties)) 
    {
		track_properties.title = guessed_title = guess_title_from_url( track_properties.url );
    }
    sd->cur_track_duration = track_properties.duration;
//	printf("Curr_duration:%d\n", sd->cur_track_duration);  
	 
	 /*  album cover  */
	xmmsc_result_t *res;
    	res = xmmsc_bindata_retrieve( sd->connection, track_properties.picture_front );
    	xmmsc_result_notifier_set_and_unref( res, _on_cover_retrieve, sd );

	elm_button_label_set(sd->artist_show, track_properties.artist);

	elm_button_label_set(sd->album_show, track_properties.album);

	elm_button_label_set(sd->title_show, track_properties.title);
	
	char time_buf[32];
	timeval_to_str( track_properties.duration/1000, time_buf, G_N_ELEMENTS(time_buf) );	
//	elm_label_label_set( sd->duration_lb, time_buf );
	edje_object_part_text_set( sd->edje, "text.duration.label", time_buf );
    
//    e_music_send_notifcation( track_properties.artist, track_properties.title );

    return TRUE;
}



static int 
_on_playback_cur_track_changed( xmmsv_t* value, void* user_data )
{
//----------------->xmmsv_get_int(value, &(sd->cur_track_id)) ID的设置已经完成？？
    if( xmmsv_get_int(value, &(sd->cur_track_id)) && (sd->cur_track_id) != 0)
    {
        xmmsc_result_t *result;
        result = xmmsc_medialib_get_info(sd->connection, sd->cur_track_id);
        xmmsc_result_notifier_set_and_unref(result, _on_playback_track_loaded, NULL);
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
	int repeat_mode = sd->repeat_mode;

    if( strncmp( key, "playlist.", 9) == 0 )
    {
        const char* val;
	g_assert( xmmsv_get_string( value, &val ) );

        if( strcmp( key + 9, "repeat_one") == 0 )
        {
            if( val[0] == '1' )
                repeat_mode = REPEAT_CURRENT;
            else
            {
                if( repeat_mode == REPEAT_CURRENT )
                    repeat_mode = REPEAT_NONE;
            }
        }
        else if( strcmp( key + 9, "repeat_all") == 0 )
        {
            if( val[0] == '1' )
                repeat_mode = REPEAT_ALL;
            else
            {
                if( repeat_mode == REPEAT_ALL )
                    repeat_mode = REPEAT_NONE;
            }
        }
//        g_signal_handlers_block_by_func(repeat_mode_cb, on_repeat_mode_changed, NULL );
//        gtk_combo_box_set_active( GTK_COMBO_BOX(repeat_mode_cb), repeat_mode );
//        g_signal_handlers_unblock_by_func(repeat_mode_cb, on_repeat_mode_changed, NULL );
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

	/* repeat mod */
	/* FIXME: The key char ??????? */
    res = xmmsc_config_get_value( sd->connection, "playlist.repeat_all" );
    xmmsc_result_notifier_set_and_unref( res, _on_cfg_repeat_all_received, sd );
    res = xmmsc_config_get_value( sd->connection, "playlist.repeat_one" );
    xmmsc_result_notifier_set_and_unref( res, _on_cfg_repeat_one_received, sd );

    res = xmmsc_config_get_value( sd->connection, "playlist.shuffle" );
    xmmsc_result_notifier_set_and_unref( res, _on_cfg_shuffle_received, sd );
    res = xmmsc_config_get_value( sd->connection, "playlist.sort" );
    xmmsc_result_notifier_set_and_unref( res, _on_cfg_sort_received, sd );

//	xmmsc_result_unref( res );
	
	return 1;
}



