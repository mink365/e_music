#ifndef __E_MUSIC_H__
#define __E_MUSIC_H__

#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <locale.h>

#define E_MUSIC_NEW(s, n) (s *)calloc(n, sizeof(s))
#define E_MUSIC_FREE(p) do { free(p); p = NULL; } while (0)

#define _(STRING) gettext(STRING)


extern int _log_dom;

#define CRITICAL(...) EINA_LOG_DOM_CRIT(_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_log_dom, __VA_ARGS__)


Evas_Object *fl;
Evas_Object *win;

typedef struct _Smart_Data Smart_Data;

struct _Smart_Data
{
	Evas_Object *layout;
	Evas_Object *edje;
	Evas_Object *cover;
	Evas_Object *slider;
	Evas_Object *artist_show;
	Evas_Object *album_show;
	Evas_Object *title_show;
	Evas_Object *play_btn;
	Evas_Object *playlist_show;
	Evas_Object *playtime_lb;
	Evas_Object *duration_lb;
	Evas_Object *repeat_btn;
	Evas_Object *shuffle_btn;
	xmmsc_connection_t *connection;
	int cur_track_duration;
	int playback_status;
	int repeat_mode;
	int shuffle_mode;
	int cur_track_id;
	//	int tmp_id;
	char *cur_playlist;
};

Smart_Data  *sd;

#endif
