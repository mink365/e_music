#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <emusic.h>

#include "playlist.h"

static int _on_playlist_content_received( xmmsv_t* value, Evas_Object *playlist_show );

void emusic_playlist_set_repeat_all(xmmsc_connection_t *conn, int i)
{
	if (i == TRUE){
		xmmsc_config_set_value(conn, "playlist.repeat_all", "1");
	}
	else {
		xmmsc_config_set_value(conn, "playlist.repeat_all", "0");
	}
}


void emusic_playlist_set_repeat_one(xmmsc_connection_t *conn, int i)
{
	if (i == TRUE){
		xmmsc_config_set_value(conn, "playlist.repeat_one", "1");
	}
	else {
		xmmsc_config_set_value(conn, "playlist.repeat_one", "0");
	}
}

void emusic_playlist_clear(xmmsc_connection_t *conn, char *playlist)
{
	xmmsc_playlist_clear(conn, playlist);
}


void 
emusic_playlist_update(xmmsc_connection_t *conn,  char *playlist)
{
	xmmsc_result_t *res;

	res = xmmsc_playlist_list_entries( conn, playlist );
	xmmsc_result_notifier_set_and_unref( res, (xmmsc_result_notifier_t)_on_playlist_content_received, NULL);

}

static int 
_on_playlist_content_received( xmmsv_t* value, Evas_Object *obj )
{
	Eina_List *list;

	int pl_size = xmmsv_list_get_size( value );
	int i;

	for ( i = 0; i < pl_size; i++ ) 
	{
		int32_t id;
		xmmsv_t *current_value;

		xmmsv_list_get( value, i, &current_value );
		xmmsv_get_int( current_value, &id );

		Emu_Media_Info *mdinfo;
		//FIXME conn???
		mdinfo = emusic_medialib_info_get(em->conn, id);

		//eina_list_append(list, mdinfo);

		emusic_playlist_view_append( em->playlist_view.genlist, mdinfo, (int *)i );
	}

	//emusic_playlist_view_update(em->playlist_view.genlist, list);

	return TRUE;
}
