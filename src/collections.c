
#include "collections.h"
#include "utils.h"

#define COLL_NAME  "e_coll"
#define COLL_NAMESPACE  "Collections"

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



