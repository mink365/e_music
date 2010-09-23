#include <xmmsclient/xmmsclient.h>
#include <glib.h>

#include "emusic.h"
#include "album_cover.h"
#include "medialib_info.h"

typedef struct _TrackProperties{
    const char *artist;
    const char *album;
    const char *title;
    const char *url;
    const char *mime;
    const char *comment;
    const char *picture_front;
    int32_t duration;
    int32_t isvbr;
    int32_t bitrate;
    int32_t size;
    int32_t id;
}TrackProperties;

static	int _get_track_properties (xmmsv_t *value, TrackProperties *properties);
static int _on_track_info_loaded(xmmsv_t* value, Emu_Media_Info *emu_mdinfo);

Emu_Media_Info *
emusic_medialib_info_get(xmmsc_connection_t *conn, int id)
{
	Emu_Media_Info *emu_mdinfo;
	emu_mdinfo = E_MUSIC_NEW(Emu_Media_Info, 1);

	xmmsc_result_t *res;
	xmmsv_t *return_value;
	res = xmmsc_medialib_get_info(conn, id);
	xmmsc_result_wait (res);
	return_value = xmmsc_result_get_value (res);
	_on_track_info_loaded(return_value, emu_mdinfo);


	xmmsv_unref (return_value);
	/*Freed???? */
	//xmmsc_result_unref (res);
	
	return emu_mdinfo;
	
}


int 
emusic_media_scan(xmmsc_connection_t *conn, char *playlist, char *file)
{
	xmmsc_result_t *res;
    char *url;

	gboolean is_dir = g_file_test( file, G_FILE_TEST_IS_DIR );

	url = g_strconcat( "file://", file, NULL );

    if( is_dir )
        res = xmmsc_playlist_radd( em->conn, em->main_playlist, url );
    else
        res = xmmsc_playlist_add_url( em->conn, em->main_playlist, url );
    g_free( url );
		
    xmmsc_result_unref( res );

	return 1;
}


/* All the result are storn in value! but how get the value, and what is the between xmmsv_t and xmmsv_result_t????  */
static	int 
_get_track_properties (xmmsv_t *value, TrackProperties *properties)  
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
		else if (strcmp( key, "id" ) == 0)
			val_int = &(properties->id);

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



static int
_on_track_info_loaded(xmmsv_t* value, Emu_Media_Info *emu_mdinfo)
{
	TrackProperties track_properties;
	const char* err;
	const char *guessed_title;

	if (xmmsv_get_error (value, &err)) {
		ERR( "Server error: %s", err );
		return FALSE;
	}

	if (!_get_track_properties( value, &track_properties)) 
	{
//		track_properties.title = guessed_title = guess_title_from_url( track_properties.url );
	}


	emu_mdinfo->artist = (char *)eina_stringshare_add(track_properties.artist);
	emu_mdinfo->album = (char *)eina_stringshare_add(track_properties.album);
	emu_mdinfo->title = (char *)eina_stringshare_add(track_properties.title);
	emu_mdinfo->duration = track_properties.duration;
	emu_mdinfo->id = track_properties.id;
	
	/*     */
	char *buf;
	buf = emusic_album_cover_path(track_properties.picture_front);
	emu_mdinfo->cover_path = buf;

	return TRUE;
	
}
