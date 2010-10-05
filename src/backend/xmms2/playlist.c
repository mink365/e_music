#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <emusic.h>

#include "playlist.h"

static int _on_playlist_content_received( xmmsv_t* value, Evas_Object *playlist_show );

void emusic_playlist_set_repeat_all(int i)
{
	if (i == TRUE){
		xmmsc_config_set_value(em->conn, "playlist.repeat_all", "1");
	}
	else {
		xmmsc_config_set_value(em->conn, "playlist.repeat_all", "0");
	}
}


void emusic_playlist_set_repeat_one(int i)
{
	if (i == TRUE){
		xmmsc_config_set_value(em->conn, "playlist.repeat_one", "1");
	}
	else {
		xmmsc_config_set_value(em->conn, "playlist.repeat_one", "0");
	}
}

void emusic_playlist_clear(char *playlist)
{
	xmmsc_playlist_clear(em->conn, playlist);
}


void 
emusic_playlist_update( char *playlist)
{
	xmmsc_result_t *res;
	xmmsv_t* val;

	em->list = eina_list_free(em->list);

	res = xmmsc_playlist_list_entries( em->conn, playlist );
	//xmmsc_result_notifier_set_and_unref( res, (xmmsc_result_notifier_t)_on_playlist_content_received, NULL); //async!!
	xmmsc_result_wait (res);
	val = xmmsc_result_get_value (res); // val still owned by res
	if (xmmsv_is_error (val)) {
		ERR ("Arg!");
		xmmsc_result_unref (res);
	}

	_on_playlist_content_received(val, NULL);

	xmmsc_result_unref (res);
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
		mdinfo = emusic_medialib_info_get(id);
		mdinfo->list_id = i;

		em->list = eina_list_append(em->list, mdinfo);
	}
	return TRUE;
}

static void
_handle_list_dicts (xmmsv_t *val) {
	xmmsv_list_iter_t *it;
	xmmsv_t *dict;
	int32_t i;
	const char *buf;

	/* Iterate over list */
	if (!xmmsv_get_list_iter (val, &it)) {
		ERR ("Oh no!");
		return;
	}
	for (xmmsv_list_iter_first (it);
	     xmmsv_list_iter_valid (it);
	     xmmsv_list_iter_next (it)) {

		if (!xmmsv_list_iter_entry (it, &dict)) {
			ERR ("Creepy!");
			return;
		}

		/* print wanted fields */
		Emu_Media_Info *mdinfo;
		mdinfo = E_MUSIC_NEW(Emu_Media_Info);
		if (!xmmsv_dict_entry_get_string (dict, "title", &buf)) {
			buf = "?????";
		}
		mdinfo->title = (char *)eina_stringshare_add(buf);
		if (!xmmsv_dict_entry_get_string (dict, "artist", &buf)) {
			buf = "?????";
		}
		mdinfo->artist = (char *)eina_stringshare_add(buf);
		if (!xmmsv_dict_entry_get_string (dict, "album", &buf)) {
			buf = "?????";
		}
		mdinfo->album = (char *)eina_stringshare_add(buf);
		if (xmmsv_dict_entry_get_string (dict, "picture_front", &buf)) {
			buf = emusic_album_cover_path(buf);
			mdinfo->cover_path = (char *)eina_stringshare_add(buf);
		}
		if (xmmsv_dict_entry_get_int (dict, "id", &i)) {
			mdinfo->id = i;
		}
		if (xmmsv_dict_entry_get_int (dict, "duration", &i)) {
			mdinfo->duration = i;
		}

		em->list = eina_list_append(em->list, mdinfo);
	}
}

Eina_List *
emusic_playlist_list_title()
{
	xmmsc_result_t *res;
	xmmsv_coll_t *univ;
	xmmsv_t *fields, *val;

	const char *fieldnames[] = {
	"title",
	"artist",
	"duration",
	NULL};

	/* request stuff */
	univ = xmmsv_coll_universe ();
	fields = xmmsv_make_stringlist ((char **)fieldnames, -1);

	res = xmmsc_coll_query_infos (em->conn, univ, fields, 0, 0, fields, fields);

	xmmsv_unref (fields);
	xmmsv_coll_unref (univ);

	/* get stuff */
	xmmsc_result_wait (res);
	val = xmmsc_result_get_value (res); // val still owned by res
	if (xmmsv_is_error (val)) {
		ERR ("Arg!");
		xmmsc_result_unref (res);

	}

	/* process stuff */
	_handle_list_dicts (val);

	xmmsc_result_unref (res);

	return em->list;
}


Eina_List *
emusic_playlist_list_album()
{
	xmmsc_result_t *res;
	xmmsv_coll_t *univ;
	xmmsv_t *fields, *val;

	const char *fieldnames[] = {
	"album",
	"title",
	"picture_front",
	NULL};

	/* request stuff */
	univ = xmmsv_coll_universe ();
	fields = xmmsv_make_stringlist ((char **)fieldnames, -1);

	res = xmmsc_coll_query_infos (em->conn, univ, fields, 0, 0, fields, fields);

	xmmsv_unref (fields);
	xmmsv_coll_unref (univ);

	/* get stuff */
	xmmsc_result_wait (res);
	val = xmmsc_result_get_value (res); // val still owned by res
	if (xmmsv_is_error (val)) {
		ERR ("Arg!");
		xmmsc_result_unref (res);

	}

	/* process stuff */
	_handle_list_dicts (val);

	xmmsc_result_unref (res);
	return em->list;
}

Eina_List *
emusic_playlist_list_artist()
{
	xmmsc_result_t *res;
	xmmsv_coll_t *univ;
	xmmsv_t *fields, *val;

	const char *fieldnames[] = {
	"artist",
	"album",
	"title",
	"id",
	"picture_front",
	NULL};

	/* request stuff */
	univ = xmmsv_coll_universe ();
	fields = xmmsv_make_stringlist ((char **)fieldnames, -1);

	res = xmmsc_coll_query_infos (em->conn, univ, fields, 0, 0, fields, fields);

	xmmsv_unref (fields);
	xmmsv_coll_unref (univ);

	/* get stuff */
	xmmsc_result_wait (res);
	val = xmmsc_result_get_value (res); // val still owned by res
	if (xmmsv_is_error (val)) {
		ERR ("Arg!");
		xmmsc_result_unref (res);
	}

	/* process stuff */
	_handle_list_dicts (val);

	xmmsc_result_unref (res);

	return em->list;
}

Eina_List *
emusic_playlist_list_playlist()
{
	xmmsc_result_t *res;
	xmmsv_t *val;
	xmmsv_t *dict;
	xmmsv_list_iter_t *it;
	char *buf;
	Emu_Media_Info *mdinfo;

	res = xmmsc_playlist_list(em->conn);
	xmmsc_result_wait (res);
	val = xmmsc_result_get_value (res);
	if (xmmsv_is_error (val)) {
		ERR("Arg!");
		xmmsc_result_unref (res);
	}

	if (!xmmsv_get_list_iter (val, &it)) {
		ERR ("Oh no!");
		return;
	}

	em->list = eina_list_free(em->list);
	for (xmmsv_list_iter_first (it);
	     xmmsv_list_iter_valid (it);
	     xmmsv_list_iter_next (it))
	{
		if (!xmmsv_list_iter_entry (it, &dict)) {
			ERR ("Creepy!");
			return;
		}
		mdinfo = E_MUSIC_NEW(Emu_Media_Info);
		
		xmmsv_get_string(dict, &buf);
		mdinfo->title = (char *)eina_stringshare_add(buf);
		em->list = eina_list_append(em->list, mdinfo);
	}

	xmmsc_result_unref (res);
	return em->list;
}

void
emusic_playlist_shuffle (char *playlist)
{
	xmmsc_result_t *res;

	res = xmmsc_playlist_shuffle(em->conn, playlist);
	xmmsc_result_wait(res);
	
	xmmsc_result_unref(res);
}

void
emusic_playlist_sort (char *playlist, char *ctx)
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
	res = xmmsc_playlist_sort(em->conn, playlist, orderval);
	xmmsc_result_wait (res);
	
	xmmsc_result_unref (res);
	xmmsv_unref (orderval);
}
