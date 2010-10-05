#ifndef __E_MUSIC_H__
#define __E_MUSIC_H__

#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <locale.h>
#include "emusic_config.h"

#define E_MUSIC_NEW(s) (s *)calloc(1, sizeof(s))
#define E_MUSIC_FREE(p) do { free(p); p = NULL; } while (0)

#define _(STRING) gettext(STRING)

int _log_dom;

#define CRITICAL(...) EINA_LOG_DOM_CRIT(_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_log_dom, __VA_ARGS__)

typedef struct _Em_Smart_Data Em_Smart_Data;

struct _Em_Smart_Data
{
	Evas_Object *win;
	Evas *evas;
	Evas_Object *layout;
	Evas_Object *edje;
	Evas_Object *pager;

	Evas_Object *mediaplayer;
	Evas_Object *config_dialog;
	Evas_Object *playlist_view;

	Evas_Object *event_bg;
	Evas_Object *cover_art;
	Evas_Object *slider;
	Evas_Object *info;
	Evas_Object *play_ctl;
	Evas_Object *playlist_ctl;
	
	xmmsc_connection_t *conn;
	int cur_track_id;

	char *main_playlist;
	char *coll_playlist;

	Eina_List *list;
};

Em_Smart_Data  *em;


enum {
    REPEAT_NONE,
    REPEAT_CURRENT,
    REPEAT_ALL,

	PLAYBACK_STATUS_PLAY,
	PLAYBACK_STATUS_PAUSE,
	PLAYBACK_STATUS_STOP,

	PLAYLIST_CHANGED_SORT,
	PLAYLIST_CHANGED_SHUFFLE
};


typedef struct _Emu_Media_Info{
	char *artist;
	char *album;
	char *title;
	char *cover_path;
	int32_t duration;
	int32_t id;
	int list_id;
}Emu_Media_Info;

#endif
