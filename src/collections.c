
#include "collections.h"
#include "utils.h"
#include "emusic.h"

#define COLL_NAME  "e_coll"
#define COLL_NAMESPACE  "Collections"

extern Smart_Data  *sd;


xmmsv_t *
create_native_value(char *value)
{
	xmmsv_t *ret;
	
	ret = xmmsv_make_stringlist ((char **)value, -1);

	return ret;
}


void 
collections_creat ( xmmsc_connection_t *conn, char *pattern )
{
	xmmsv_coll_t *coll = NULL;
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;
	

	if (!xmmsv_coll_parse (pattern, &coll)) {
		print_error ("Unable to generate query");
	}


	res = xmmsc_coll_save (conn, coll, COLL_NAME, COLL_NAMESPACE);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		print_error ("Couldn't save %s in namespace %s: %s", COLL_NAME, COLL_NAMESPACE,
		             errmsg);
	}
	
	xmmsc_result_unref (res);
	xmmsv_coll_unref (coll);
}

void 
collections_add_to_playlist( xmmsc_connection_t *conn, char *playlist)
{
	xmmsv_coll_t *collref;
	xmmsc_result_t *res;
	xmmsv_t *val;
	const char *errmsg;

	gchar **orderv = NULL;
	xmmsv_t *order;

	printf("Add the collection to playlist!!\n");

	/* We have to give a order  , don't make it NULL*/
	order = xmmsv_make_stringlist (orderv, -1);
	if (orderv) {
		g_strfreev (orderv);
	}

	/* Create a reference collection to the saved coll */
	collref = xmmsv_coll_new (XMMS_COLLECTION_TYPE_REFERENCE);
	xmmsv_coll_attribute_set (collref, "reference", COLL_NAME);
	xmmsv_coll_attribute_set (collref, "namespace", COLL_NAMESPACE);

	res = xmmsc_playlist_add_collection (conn, playlist, collref, order);
	xmmsc_result_wait (res);

	val = xmmsc_result_get_value (res);
	if (xmmsv_get_error (val, &errmsg)) {
		print_error ("%s", errmsg);
	}

	xmmsc_result_unref (res);

	xmmsv_coll_unref (collref);	
}


static void _update_artists(xmmsv_t *val)
{
	xmmsv_list_iter_t *it;
	xmmsv_t *dict;
	const char *buf;
	gchar        *artist, *album;
	gint          id;
	gchar         artist_tmp[192] = "", album_tmp[192] = "";


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
		if (!xmmsv_dict_entry_get_string (dict, "artist", &buf)) {
//			buf = "?????";
		}
		INF ("%s", buf);

		if (!xmmsv_dict_entry_get_string (dict, "album", &buf)) {
//			buf = "?????";
		}
		INF ("%s", buf);

		if (!xmmsv_dict_entry_get_string (dict, "picture_front", &buf)) {
//			buf = "?????";
		}
		INF ("%s", buf);

	}

}


void emusic_mlib_browse_update_artists()
{
	xmmsc_result_t *res;
	xmmsv_t *return_value;
	xmmsc_coll_t   *universe;
	const gchar    *properties[] = { "artist", "album", "picture_front", NULL };
	const gchar    *group_by[]   = { "album", NULL };


	universe = xmmsc_coll_universe();

	
	res = xmmsc_coll_query_infos(sd->connection, universe, create_native_value( group_by ),
	                             0, 0, create_native_value( properties ), create_native_value( group_by ));
	xmmsv_coll_unref (universe);
	
	xmmsc_result_wait (res);
	return_value = xmmsc_result_get_value (res);
	
	_update_artists(return_value);

//	xmmsv_unref (return_value);
	xmmsc_result_unref(res);
}


