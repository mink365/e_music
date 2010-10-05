#include <Elementary.h>
#include <emusic.h>

#include "emusic_playlist_view.h"

static const char _EDJE_GROUP_PLAYLIST[] = "main/playlist_view";
extern Em_Smart_Data  *em;

typedef struct _Smart_Data
{
	Evas_Object *genlist;
	Evas_Object *bx;
} Smart_Data;
static Smart_Data *sd;

typedef struct _Listitem
{
	char *label1;
	char *label2;
	char *label3;
	char *cover_path;
	int32_t id;

	Eina_List *list;
} Listitem;

static Elm_Genlist_Item_Class itclass;
static Elm_Genlist_Item_Class subitclass;

static void _playlist_toolbar_clear();

static void _tb_artist_clicked(void *data, Evas_Object *obj, void *event_info);
static void _tb_album_clicked(void *data, Evas_Object *obj, void *event_info);
static void _tb_title_clicked(void *data, Evas_Object *obj, void *event_info);
static void _tb_playlist_clicked(void *data, Evas_Object *obj, void *event_info);
static void _tb_player_clicked(void *data, Evas_Object *obj, void *event_info);
static void _tb_title_append(Evas_Object *gl, Emu_Media_Info *mdinfo, int list_id);
static void _tb_artist_append(Evas_Object *gl, Listitem *item, int list_id);
static void _tb_album_append(Evas_Object *gl, Listitem *item, int list_id);
static void _tb_playlist_append(Evas_Object *gl, Emu_Media_Info *mdinfo, int list_id);

int 
emusic_playlist_toolbar_creat(Em_Smart_Data *em)
{
	Evas_Object *tb, *genlist, *obj, *bx, *ic;
	char buf[PATH_MAX];

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;
	
	obj = edje_object_add(em->evas);
	if (!edje_object_file_set(obj, emusic_config_theme_get(), _EDJE_GROUP_PLAYLIST))
	{
		int err = edje_object_load_error_get(obj);
		const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),_EDJE_GROUP_PLAYLIST, errmsg);
    	evas_object_del(obj);
    	return 0;
	}
  	em->playlist_view = obj;

	bx = elm_box_add(obj);
	evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	edje_object_part_swallow(obj, "playlist_view.swallow", bx);
	evas_object_show(bx);
	sd->bx = bx;

	tb = elm_toolbar_add(obj);
   	elm_toolbar_homogenous_set(tb, 0);
   	evas_object_size_hint_weight_set(tb, 0.0, 0.0);
   	evas_object_size_hint_align_set(tb, EVAS_HINT_FILL, 0.0);
	evas_object_show(tb);
	elm_box_pack_end(bx, tb);

	ic = elm_icon_add(obj);
   	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/artist");
   	elm_toolbar_item_add(tb, ic, _("Artist"), _tb_artist_clicked, NULL);

	ic = elm_icon_add(obj);
   	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/album");
   	elm_toolbar_item_add(tb, ic, _("Album"), _tb_album_clicked, NULL);

	ic = elm_icon_add(obj);
   	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/songs");
   	elm_toolbar_item_add(tb, ic, _("Songs"), _tb_title_clicked, NULL);

	ic = elm_icon_add(obj);
   	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/playlist");
   	elm_toolbar_item_add(tb, ic, _("Playlist"), _tb_playlist_clicked, NULL);

	ic = elm_icon_add(obj);
   	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_play");
   	elm_toolbar_item_add(tb, ic, _("Player"), _tb_player_clicked, NULL);

	genlist = elm_genlist_add(obj);
	sd->genlist = genlist;
	evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(genlist);
	elm_box_pack_end(bx, genlist);

	return 1;

}


static char *
_gl_album_label_get(const void *data, Evas_Object *obj, const char *part)
{
  	const  Listitem *tit = data;

	if (!tit)
		return NULL;

	char buf[PATH_MAX];
    if (!strcmp(part, "elm.text"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->label1);
	
	}
	else if (!strcmp(part, "elm.text.sub"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->label2);
	}
	else {
		snprintf(buf, sizeof(buf), "%s", "");
	}
	return strdup(buf);
}

static char *
_gl_title_label_get(const void *data, Evas_Object *obj, const char *part)
{
  	const  Listitem *tit = data;

	if (!tit)
		return NULL;

	char buf[PATH_MAX];
    if (!strcmp(part, "elm.text"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->label1);
	
	}
	else if (!strcmp(part, "elm.text.sub"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->label2);
	}
	else if (!strcmp(part, "elm.text.end"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->label3);
	}
	else {
		snprintf(buf, sizeof(buf), "%s", "");
	}
	return strdup(buf);
}

static Evas_Object *
_gl_title_icon_get(const void *data, Evas_Object *obj, const char *part)
{
	const  Listitem *tit = data;	

	Evas_Object *ic = elm_icon_add(obj);
	if (!strcmp(part, "elm.swallow.end"))
	{
		if (tit->id == em->cur_track_id)
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/playing");
	}

	return ic;
}

static Evas_Object *
_gl_playlist_icon_get(const void *data, Evas_Object *obj, const char *part)
{
	const  Listitem *tit = data;	

	Evas_Object *ic = elm_icon_add(obj);
	if (!strcmp(part, "elm.swallow.icon"))
	{
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/mp_playlist");
	}

	return ic;
}

static Evas_Object *
_gl_album_icon_get(const void *data, Evas_Object *obj, const char *part)
{
	const  Listitem *tit = data;

	Evas_Object *ic = elm_icon_add(obj);
	if (!strcmp(part, "elm.swallow.icon"))
	{
		if (tit->cover_path) {
			elm_icon_file_set(ic, tit->cover_path, NULL);
		}
		else {
			INF("No cover picture for this Album.");
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/no_cover");
		}
	}
	/*We have to add it ... */
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	return ic;
}

Eina_Bool
_gl_state_get(const void *data, Evas_Object *obj, const char *part)
{
	return EINA_FALSE;
}

void
_gl_del(const void *data, Evas_Object *obj, const char *part)
{


}

static void
_list_sel(void *data, Evas_Object *obj, void *event_info)
{
	const int list_id = (int)data;

	//ERR("%d", list_id);

	emusic_playback_jump(list_id);

    if (emusic_play_menu_creat(em)) {
        elm_pager_content_push(em->pager, em->mediaplayer);
    }
    else
        ERR("could not create main menu.\n");

	emusic_playback_play();
}

static void
_list_album_sel(void *data, Evas_Object *obj, void *event_info)
{
	char *album;
	char buf[PATH_MAX] = "album:'";

	album = (char *)data;

	emusic_playlist_view_new(album);
}

static void
gl_exp(void *data, Evas_Object *obj, void *event_info)
{
	//INF("gl_exp....");
	Eina_List *l;
	Listitem *item;
   	Elm_Genlist_Item *it = event_info;
   	Evas_Object *gl = elm_genlist_item_genlist_get(it);
	Listitem *listitem = elm_genlist_item_data_get(it);

	if (listitem->id == (int)data) {
		EINA_LIST_FOREACH(listitem->list, l, item) {
			Listitem *listitem;
			char buf[PATH_MAX];
			//INF("%s", item->label1);

			snprintf(buf, sizeof(buf), "%d songs", eina_list_count(item->list));

   			subitclass.item_style     = "three_label";
			subitclass.func.label_get = _gl_album_label_get;
			subitclass.func.icon_get  = _gl_album_icon_get;
			subitclass.func.state_get = NULL;
			subitclass.func.del       = NULL;

			item->label2 = eina_stringshare_add(buf);
			elm_genlist_item_append(gl, &subitclass,
									item, it, ELM_GENLIST_ITEM_NONE, _list_album_sel, item->label1);
		}
	}
}
static void
gl_con(void *data, Evas_Object *obj, void *event_info)
{
	INF("gl con....");
   Elm_Genlist_Item *it = event_info;
   elm_genlist_item_subitems_clear(it);
}

static void
gl_exp_req(void *data, Evas_Object *obj, void *event_info)
{
	INF("gl_exp req....");
   Elm_Genlist_Item *it = event_info;
   elm_genlist_item_expanded_set(it, 1);
}
static void
gl_con_req(void *data, Evas_Object *obj, void *event_info)
{
	INF("gl con req....");
   Elm_Genlist_Item *it = event_info;
   elm_genlist_item_expanded_set(it, 0);
}


/////////////////////////////////////////////
//toolbar
static void
_tb_artist_clicked(void *data, Evas_Object *obj, void *event_info)
{
	Eina_List *l, *list;
	Emu_Media_Info *mdinfo;
	Eina_List *artist_list = NULL;
	Listitem *artist = NULL, *album = NULL, *title = NULL, *listitem = NULL;

	_playlist_toolbar_clear();

	list = (Eina_List *)emusic_playlist_list_artist();

	EINA_LIST_FOREACH(em->list, l, mdinfo) {
		INF("%s, %s, %s, %d", mdinfo->artist, mdinfo->album, mdinfo->title, mdinfo->id);
		if (artist_list) {
			artist = eina_list_data_get(eina_list_last(artist_list));
			if (!artist) continue;
			if (!strncmp(mdinfo->artist, artist->label1, sizeof(mdinfo->artist))) {
				album = eina_list_data_get(eina_list_last(artist->list));
				if (!album) continue;
				if (!strncmp(mdinfo->album, album->label1, sizeof(mdinfo->album))) {
					title = E_MUSIC_NEW(Listitem);
					title->label1 = mdinfo->title;
					title->id = mdinfo->id;
					album->list = eina_list_append(album->list, title);
				}
				else {
					album = E_MUSIC_NEW(Listitem);
					album->label1 = mdinfo->album;
					album->cover_path = mdinfo->cover_path;

					title = E_MUSIC_NEW(Listitem);
					title->label1 = mdinfo->title;
					title->id = mdinfo->id;

					album->list = eina_list_append(album->list, title);
					artist->list = eina_list_append(artist->list, album);
				}
			}
			else {
				artist = E_MUSIC_NEW(Listitem);
				artist->label1 = mdinfo->artist;
				artist->id = mdinfo->id;

				album = E_MUSIC_NEW(Listitem);
				album->label1 = mdinfo->album;
				album->cover_path = mdinfo->cover_path;

				title = E_MUSIC_NEW(Listitem);
				title->label1 = mdinfo->title;
				title->id = mdinfo->id;

				album->list = eina_list_append(album->list, title);
				artist->list = eina_list_append(artist->list, album);
				artist_list = eina_list_append(artist_list, artist);
			}
		}
		else {
			INF("INIT >>>");
			artist = E_MUSIC_NEW(Listitem);
			artist->label1 = mdinfo->artist;
			artist->id = mdinfo->id;

			album = E_MUSIC_NEW(Listitem);
			album->label1 = mdinfo->album;
			album->cover_path = mdinfo->cover_path;

			title = E_MUSIC_NEW(Listitem);
			title->label1 = mdinfo->title;
			title->id = mdinfo->id;

			album->list = eina_list_append(album->list, title);
			artist->list = eina_list_append(artist->list, album);
			artist_list = eina_list_append(artist_list, artist);
		}
	}
	eina_list_free (em->list);
	em->list = NULL;

	EINA_LIST_FOREACH(artist_list, l, listitem) {
		_tb_artist_append(sd->genlist, listitem, 1);
	}
	eina_list_free(artist_list);
}

static void
_tb_album_clicked(void *data, Evas_Object *obj, void *event_info)
{
	Eina_List *l, *list;
	Emu_Media_Info *mdinfo;
	Eina_List *album_list = NULL;
	Listitem *listitem = NULL, *item = NULL;
	int i;

	_playlist_toolbar_clear();

	list = (Eina_List *)emusic_playlist_list_album();

	EINA_LIST_FOREACH (em->list, l, mdinfo) {
		if (album_list) {
			listitem = eina_list_data_get(eina_list_last(album_list));
			if (!listitem) continue;
			if (!strncmp(mdinfo->album, listitem->label1, sizeof(mdinfo->album))) {
				//INF("remve songs!! %s", mdinfo->album);

				item = E_MUSIC_NEW(Listitem);
				item->label1 = mdinfo->title;
				listitem->list = eina_list_append(listitem->list, item);
			}
			else {
				listitem = E_MUSIC_NEW(Listitem);
				listitem->label1 = mdinfo->album;
				listitem->cover_path = mdinfo->cover_path;

				item = E_MUSIC_NEW(Listitem);
				item->label1 = mdinfo->title;
				listitem->list = eina_list_append(listitem->list, item);
				album_list = eina_list_append(album_list, listitem);
			}
		}
		else {
			INF("new listitem!!");
			listitem = E_MUSIC_NEW (Listitem);
			listitem->label1 = mdinfo->album;
			listitem->cover_path = mdinfo->cover_path;

			item = E_MUSIC_NEW (Listitem);
			item->label1 = mdinfo->title;
			listitem->list = eina_list_append (listitem->list, item);
			album_list = eina_list_append (album_list, listitem);
		}
	}
	eina_list_free (em->list);
	em->list = NULL;

	EINA_LIST_FOREACH (album_list, l, listitem) {
		_tb_album_append (sd->genlist, listitem, 1);
	}

	return;
}

static void
_tb_title_clicked(void *data, Evas_Object *obj, void *event_info)
{
	Eina_List *l, *list;
	Emu_Media_Info *mdinfo;

	_playlist_toolbar_clear();

	char buf[PATH_MAX] = "title:'";

	strcat(buf, "*");
	strcat(buf, "'");

	emusic_collections_creat(buf );

	emusic_playlist_clear(em->coll_playlist);

	emusic_collections_add_to_playlist(em->coll_playlist);

	emusic_playlist_update(em->main_playlist);

	EINA_LIST_FOREACH(em->list, l, mdinfo) {
		_tb_title_append(sd->genlist, mdinfo, 1);
	}
	eina_list_free(em->list);
	em->list = NULL;
}

static void
_tb_playlist_clicked(void *data, Evas_Object *obj, void *event_info)
{
	Eina_List *l, *list;
	Emu_Media_Info *mdinfo;

	_playlist_toolbar_clear();

	list = (Eina_List *)emusic_playlist_list_playlist();

	EINA_LIST_FOREACH(em->list, l, mdinfo) {
		_tb_playlist_append(sd->genlist, mdinfo, 1);
	}
	eina_list_free(em->list);
	em->list = NULL;
}

static void
_tb_player_clicked(void *data, Evas_Object *obj, void *event_info)
{
	if (emusic_play_menu_creat(em)) {
		elm_pager_content_push(em->pager, em->mediaplayer);
	}
	else
		ERR("could not create main menu.\n");
}


static void
_tb_title_append(Evas_Object *gl, Emu_Media_Info *mdinfo, int list_id)
{
	Listitem *listitem;
	char time_buf[32];
	utils_timeval_to_str(mdinfo->duration/1000, time_buf, sizeof(time_buf));

   	itclass.item_style     = "three_label";
	itclass.func.label_get = _gl_title_label_get;
	itclass.func.icon_get  = _gl_title_icon_get;
	itclass.func.state_get = NULL;
	itclass.func.del       = NULL;

	listitem = E_MUSIC_NEW(Listitem);
	listitem->label1 = mdinfo->title;
	listitem->label2 = mdinfo->artist;
	listitem->label3 = eina_stringshare_add(time_buf);
	listitem->id = mdinfo->id;
	elm_genlist_item_append(gl, &itclass,
		listitem, NULL/* parent */, ELM_GENLIST_ITEM_NONE, _list_sel, (int *)mdinfo->list_id/* func data */ );

}

static void
_tb_artist_append(Evas_Object *gl, Listitem *item, int list_id)
{
	Listitem *listitem;
	char buf[PATH_MAX];

	snprintf(buf, sizeof(buf), "%d album", eina_list_count(item->list));

   	itclass.item_style     = "three_label";
	itclass.func.label_get = _gl_album_label_get;
	itclass.func.icon_get  = NULL;
	itclass.func.state_get = NULL;
	itclass.func.del       = NULL;

	item->label2 = eina_stringshare_add(buf);
	elm_genlist_item_append(gl, &itclass,
		item, NULL/* parent */, ELM_GENLIST_ITEM_SUBITEMS, NULL, (int *)item->id/* func data */ );

   	evas_object_smart_callback_add(gl, "expand,request", gl_exp_req, gl);
   	evas_object_smart_callback_add(gl, "contract,request", gl_con_req, gl);
   	evas_object_smart_callback_add(gl, "expanded", gl_exp, (int *)item->id);
   	evas_object_smart_callback_add(gl, "contracted", gl_con, gl);
}

static void
_tb_album_append(Evas_Object *gl, Listitem *item, int list_id)
{
	Listitem *listitem;
	char buf[PATH_MAX];

	snprintf(buf, sizeof(buf), "%d songs", eina_list_count(item->list));

   	itclass.item_style     = "three_label";
	itclass.func.label_get = _gl_album_label_get;
	itclass.func.icon_get  = _gl_album_icon_get;
	itclass.func.state_get = NULL;
	itclass.func.del       = NULL;

	item->label2 = eina_stringshare_add(buf);
	elm_genlist_item_append(gl, &itclass,
		item, NULL/* parent */, ELM_GENLIST_ITEM_NONE, _list_album_sel, item->label1/* func data */ );

}

static void
_tb_playlist_append(Evas_Object *gl, Emu_Media_Info *mdinfo, int list_id)
{
	Listitem *listitem;

   	itclass.item_style     = "default";
	itclass.func.label_get = _gl_album_label_get;
	itclass.func.icon_get  = _gl_playlist_icon_get;
	itclass.func.state_get = NULL;
	itclass.func.del       = NULL;

	listitem = E_MUSIC_NEW(Listitem);
	listitem->label1 = mdinfo->title;
	elm_genlist_item_append(gl, &itclass, 
		listitem, NULL/* parent */, ELM_GENLIST_ITEM_NONE, NULL, NULL/* func data */ );

}

void
_playlist_toolbar_clear()
{
	//elm_genlist_clear(sd->genlist);  //FIXME: Can't clear subitems, so ,just creat a new one.

	evas_object_del(sd->genlist);

	sd->genlist = elm_genlist_add(em->playlist_view);
	evas_object_size_hint_align_set(sd->genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(sd->genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(sd->genlist);

	elm_box_pack_end(sd->bx, sd->genlist);
}


int
emusic_playlist_view_new(char *album)
{
	Evas_Object *fr, *genlist;
	char buf[PATH_MAX] = "album:'";
	Eina_List *l, *list;
	Emu_Media_Info *mdinfo;

	fr = elm_frame_add(em->win);
	elm_object_style_set(fr, "emusic");
   	evas_object_size_hint_align_set(fr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   	evas_object_size_hint_weight_set(fr, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   	evas_object_show(fr);

   	genlist = elm_genlist_add(em->win);
   	evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_frame_content_set(fr, genlist);
	evas_object_show(genlist);

	if (album)
		elm_frame_label_set(fr, album);

	EINA_LIST_FOREACH(em->list, l, mdinfo) {
		_tb_title_append(genlist, mdinfo, 1);
	}
	eina_list_free(em->list);
	em->list = NULL;

	elm_pager_content_push(em->pager, fr);

	return TRUE;
}

