#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <emusic.h>

#include "playback.h"

typedef struct _Smart_Data
{
	xmmsc_connection_t *conn;
} Smart_Data;
static Smart_Data *sd;

static void _do_reljump (xmmsc_connection_t *conn, int where);

int 
emusic_playback_init(Em_Smart_Data *em)
{
	xmmsc_connection_t *conn;
	xmmsc_result_t *result;
	xmmsv_t *return_value;
	char *path;
	int *ret;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;

	conn = xmmsc_init ("e_music");

	if (!conn) {
		ERR ("Could not init xmmsc_connection!");
	}
	sd->conn = conn;
	em->conn = conn;

//---could not get the PATH????
	path = getenv ("XMMS_PATH");
	if (!path) {
//		ERR ( "Could get PATH");
	}
		
	ret = xmmsc_connect (conn, path);
	if (!ret)
	{
		if (!system("xmms2-launcher"))
			ret = xmmsc_connect (conn, path);
	}	

	if ( !ret ) {
		ERR ("Connection failed: %s\n",
		         xmmsc_get_last_error (conn));
		xmmsc_unref (conn);
	}

	xmmsc_mainloop_ecore_init(conn);
	
	return TRUE;
}

void
emusic_playback_play()
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playback_start (sd->conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);

	if (xmmsv_get_error (val, &errmsg)) {
		ERR ("Couldn't start playback: %s", errmsg);
	}

	xmmsc_result_unref (res);
}

void
emusic_playback_pause()
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	res = xmmsc_playback_pause (sd->conn);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		ERR ("Couldn't start playback: %s", errmsg);
	}
	xmmsc_result_unref (res);
}

void
emusic_playback_prev()
{
	_do_reljump (sd->conn, -1);
}

void
emusic_playback_next()
{	
	_do_reljump (sd->conn, 1);
}

void
emusic_playback_seek(uint32_t time)
{
	xmmsc_result_t* res;

	res = xmmsc_playback_seek_ms( sd->conn, time, XMMS_PLAYBACK_SEEK_SET );

	xmmsc_result_unref(res);
}

void 
emusic_playback_jump(int id)
{
	_do_reljump (sd->conn, id);
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
