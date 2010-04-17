

#include <stdlib.h>
#include "playback.h"
#include "utils.h"


static void
_do_reljump (xmmsc_connection_t *conn, gint where)
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playlist_set_next_rel (conn, where);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		printf ("Couldn't advance in playlist: %s", errmsg);
	}
	xmmsc_result_unref (res);

	res = xmmsc_playback_tickle (conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		printf ("%s", errmsg);
	}

	xmmsc_result_unref (res);
}


int 
playback_init(Smart_Data *sd)
{

	xmmsc_result_t *result;
	xmmsv_t *return_value;
	gchar *path;
	gint *ret;

	sd->connection = xmmsc_init ("e_music");

	if (!sd->connection) {
		print_error ("Could not init xmmsc_connection!");
		
		exit (EXIT_FAILURE);
	}

//---could not get the PATH????
	path = getenv ("XMMS_PATH");
	if (!path) {
//		print_error ( "Could get PATH");
	}
		
	ret = xmmsc_connect (sd->connection, path);
	if (!ret)
	{
		if (!system("xmms2-launcher"))
			ret = xmmsc_connect (sd->connection, path);
	}	

	if ( !ret ) {
		print_error ("Connection failed: %s\n",
		         xmmsc_get_last_error (sd->connection));
		xmmsc_unref (sd->connection);
		exit (EXIT_FAILURE);
	}
	
/*    get current track ID    */
	result = xmmsc_playback_current_id (sd->connection);
	xmmsc_result_wait (result);
	return_value = xmmsc_result_get_value (result);
	xmmsv_get_int (return_value, &(sd->cur_track_id));

	//----------初始化状态，有无必要？？？ 
//	sd->playback_status = XMMS_PLAYBACK_STATUS_STOP;

	xmmsc_mainloop_ecore_init(sd->connection);
	
	return 1;
}

void
playback_play(xmmsc_connection_t *conn)
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playback_start (conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);

	if (xmmsv_get_error (val, &errmsg)) {
		print_error ("Couldn't start playback: %s", errmsg);
	}

	xmmsc_result_unref (res);
}

void
playback_pause(xmmsc_connection_t *conn)
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playback_pause (conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		print_error ("Couldn't start playback: %s", errmsg);
	}
	xmmsc_result_unref (res);
}

void
playback_prev(xmmsc_connection_t *conn)
{
	_do_reljump (conn, -1);
}

void
playback_next(xmmsc_connection_t *conn)
{	
	_do_reljump (conn, 1);
}



void
emusic_playlist_shuffle (xmmsc_connection_t *conn, char *playlist)
{
	xmmsc_result_t *res;

	res = xmmsc_playlist_shuffle (conn, playlist);
	xmmsc_result_wait (res);
	
	xmmsc_result_unref (res);
	
}

void
emusic_playlist_sort (xmmsc_connection_t *conn, char *playlist, char *ctx)
{
	xmmsc_result_t *res;
	xmmsv_t *orderval;
	char **order = NULL;
	const char *default_order[] = { "artist", "album", "tracknr", NULL};


	if ( !ctx ) {
		order = default_order;
	} else {
		order = ctx;
	}

	orderval = xmmsv_make_stringlist (order, -1);
	res = xmmsc_playlist_sort (conn, playlist, orderval);
	xmmsc_result_wait (res);
	
	xmmsc_result_unref (res);
	xmmsv_unref (orderval);

}

