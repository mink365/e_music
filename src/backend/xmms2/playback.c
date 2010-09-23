#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <emusic.h>

#include "playback.h"

static void _do_reljump (xmmsc_connection_t *conn, int where);

int 
emusic_playback_init(Em_Smart_Data *em)
{

	xmmsc_result_t *result;
	xmmsv_t *return_value;
	char *path;
	int *ret;

	em->conn = xmmsc_init ("e_music");

	if (!em->conn) {
		ERR ("Could not init xmmsc_connection!");
		
		exit (EXIT_FAILURE);
	}

//---could not get the PATH????
	path = getenv ("XMMS_PATH");
	if (!path) {
//		ERR ( "Could get PATH");
	}
		
	ret = xmmsc_connect (em->conn, path);
	if (!ret)
	{
		if (!system("xmms2-launcher"))
			ret = xmmsc_connect (em->conn, path);
	}	

	if ( !ret ) {
		ERR ("Connection failed: %s\n",
		         xmmsc_get_last_error (em->conn));
		xmmsc_unref (em->conn);
		exit (EXIT_FAILURE);
	}
	
/*    get current track ID    */
	result = xmmsc_playback_current_id (em->conn);
	xmmsc_result_wait (result);
	return_value = xmmsc_result_get_value (result);
	xmmsv_get_int (return_value, &(em->cur_track_id));

	//----------初始化状态，有无必要？？？ 
//	em->playback_status = XMMS_PLAYBACK_STATUS_STOP;

	xmmsc_mainloop_ecore_init(em->conn);
	
	return 1;
}

void
emusic_playback_play(xmmsc_connection_t *conn)
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playback_start (conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);

	if (xmmsv_get_error (val, &errmsg)) {
		ERR ("Couldn't start playback: %s", errmsg);
	}

	xmmsc_result_unref (res);
}

void
emusic_playback_pause(xmmsc_connection_t *conn)
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playback_pause (conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		ERR ("Couldn't start playback: %s", errmsg);
	}
	xmmsc_result_unref (res);
}

void
emusic_playback_prev(xmmsc_connection_t *conn)
{
	_do_reljump (conn, -1);
}

void
emusic_playback_next(xmmsc_connection_t *conn)
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

void
emusic_playback_seek(xmmsc_connection_t *conn, uint32_t time)
{
	xmmsc_result_t* res;

	res = xmmsc_playback_seek_ms( conn, time, XMMS_PLAYBACK_SEEK_SET );

	xmmsc_result_unref(res);
}

void emusic_playback_jump(xmmsc_connection_t *conn, int id)
{
		xmmsc_result_t* res;
	res = xmmsc_playlist_set_next(conn, id);
	xmmsc_result_unref(res);

	res = xmmsc_playback_tickle(conn);
	xmmsc_result_unref(res);
}


static void
_do_reljump (xmmsc_connection_t *conn, int where)
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
