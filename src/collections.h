#ifndef __COLLECTIONS_H__
#define __COLLECTIONS_H__

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>

#include <glib.h>
#include <string.h>


void collections_creat ( xmmsc_connection_t *conn, char *pattern );

void collections_add_to_playlist( xmmsc_connection_t *conn, char *playlist);

#endif
