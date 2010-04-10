/*
 *      utils.h
 *      
 *      Copyright 2008 PCMan <pcman.tw@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#ifndef __LXMUSIC_UTILS_H__
#define __LXMUSIC_UTILS_H__

#include <glib.h>

#include <xmmsclient/xmmsclient.h>

G_BEGIN_DECLS

void kf_get_bool(GKeyFile* kf, const char* grp, const char* key, gboolean* val);
void kf_get_int(GKeyFile* kf, const char* grp, const char* key, int* val);

gchar* utf8_strcasestr( const gchar* s1, const gchar* s2 );

const char* timeval_to_str( guint timeval, char* buf, guint buf_len );

gchar* guess_title_from_url (const char *url);


void xmmsc_result_notifier_set_and_unref (xmmsc_result_t *res, xmmsc_result_notifier_t func, void *user_data);

void print_error (const gchar *fmt, ...);


G_END_DECLS

#endif
