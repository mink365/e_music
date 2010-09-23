#ifndef __E_MUSIC_H__
#define __E_MUSIC_H__

#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <locale.h>
#include "emusic_config.h"

#define E_MUSIC_NEW(s, n) (s *)calloc(n, sizeof(s))
#define E_MUSIC_FREE(p) do { free(p); p = NULL; } while (0)

#define _(STRING) gettext(STRING)

int _log_dom;

#define CRITICAL(...) EINA_LOG_DOM_CRIT(_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_log_dom, __VA_ARGS__)

Evas_Object *win;

typedef struct _Em_Smart_Data Em_Smart_Data;

struct _Em_Smart_Data
{
	Evas *evas;
	Evas_Object *layout;
	Evas_Object *edje;

	Evas_Object *mediaplayer;
	struct {
		Evas_Object *obj;
		Evas_Object *toolbar;
		Evas_Object *fs_bt;
		Evas_Object *file_toggle;
		Evas_Object *file_entry;
	
		char *file;
	} config_dialog;
	struct {
		Evas_Object *obj;
  		int    mouse_act;
  		int    mouse_x;
  		int    mouse_y;
  		int    mouse_button;
	} event_bg;
	struct {
		Evas_Object *obj;
		Evas_Object *cover;
	} cover_art;
	struct {
		Evas_Object *obj;
  		Evas_Object *slider;

		int cur_track_duration;
		double playtime;
		int slider_runing;
	} slider;
	struct {
		Evas_Object *obj;
  		Evas_Object *artist_show;
  		Evas_Object *album_show;
  		Evas_Object *title_show;
	} info;
	struct {
		Evas_Object *obj;
  		Evas_Object *play_btn;
	} play_ctl;
	struct {
		Evas_Object *obj;
		Evas_Object *genlist;
	} playlist_view;
	struct {
		Evas_Object *obj;
		Evas_Object *repeat_btn;
		Evas_Object *shuffle_btn;
	} playlist_ctl;
	
	struct {
		Evas_Object *label;
		Evas_Object *notify;
	} notify;
	
	xmmsc_connection_t *conn;
	
	int playback_status;
	int repeat_mode;
	int shuffle_mode;
	int cur_track_id;
	char *main_playlist;
	char *coll_playlist;
};

Em_Smart_Data  *em;


enum {
    REPEAT_NONE,
    REPEAT_CURRENT,
    REPEAT_ALL
};


typedef struct _Emu_Media_Info{
	char *artist;
	char *album;
	char *title;
	char *cover_path;
	int32_t duration;
	int32_t id;
}Emu_Media_Info;

#endif
