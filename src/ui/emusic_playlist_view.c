#include <Elementary.h>
#include <emusic.h>

#include "emusic_playlist_view.h"

static const char _EDJE_GROUP_PLAYLIST[] = "main/playlist_view";

extern Em_Smart_Data  *em;

typedef struct _Smart_Data Smart_Data;

struct _Smart_Data
{
	Evas_Object *genlist;
};

typedef struct _Listitem
{
	 Emu_Media_Info *mdinfo;
} Listitem;

static Elm_Genlist_Item_Class itclass;


int 
emusic_playlist_view_creat(Em_Smart_Data *em)
{
	Evas_Object *tb, *genlist, *obj;

	Smart_Data *sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return;
	
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
	//evas_object_smart_data_set(obj, sd);
  	em->playlist_view.obj = obj;

	tb = elm_table_add(obj);	
	evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	edje_object_part_swallow(obj, "playlist_view.swallow", tb);
	evas_object_show(tb);

	genlist = elm_genlist_add(obj);
	em->playlist_view.genlist = genlist;
	evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(genlist);	
	elm_table_pack(tb, genlist, 1, 0, 1, 1);

	return 1;

}


char *
_gl_label_get(const void *data, Evas_Object *obj, const char *part)
{
  	const  Listitem *tit = data;

	if (!tit || !tit->mdinfo)
		return NULL;

      	char buf[PATH_MAX];
       	if (!strcmp(part, "elm.text"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->mdinfo->title);
	
	}
	else if (!strcmp(part, "elm.text.sub"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->mdinfo->artist);
	}
	return strdup(buf);

}

Evas_Object *
_gl_icon_get(const void *data, Evas_Object *obj, const char *part)
{
	const  Listitem *tit = data;	

	Evas_Object *ic = elm_icon_add(obj);
	if (!strcmp(part, "elm.swallow.icon"))
	{
		elm_icon_file_set(ic, emusic_config_theme_get(), "icon/music");
	}
	else if (!strcmp(part, "elm.swallow.end"))
	{
		if (tit->mdinfo->id == em->cur_track_id)
			elm_icon_file_set(ic, emusic_config_theme_get(), "icon/playing");
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

	emusic_playback_jump(em->conn, list_id);
	/*              */      
	emusic_switch_to_mediaplayer();
}

void
emusic_playlist_view_append(Evas_Object *playlist_show, Emu_Media_Info *mdinfo, int list_id)
{
	Listitem *listitem;	

	INF("genlist//////%d, %d", mdinfo->id, list_id);
   	itclass.item_style     = "double_label";
	itclass.func.label_get = _gl_label_get;
	itclass.func.icon_get  = _gl_icon_get;
	itclass.func.state_get = NULL;
	itclass.func.del       = NULL;

	listitem = E_MUSIC_NEW(Listitem, 1);
	listitem->mdinfo = mdinfo;
	elm_genlist_item_append(playlist_show, &itclass, 
		listitem, NULL/* parent */, ELM_GENLIST_ITEM_NONE, _list_sel, (int *)list_id/* func data */ );

}

void
emusic_playlist_view_update(Evas_Object *genlist, Eina_List *list)
{
	Eina_List *l;
	Emu_Media_Info *mdinfo;

	EINA_LIST_FOREACH(list, l, mdinfo) {
		emusic_playlist_view_append(genlist, mdinfo, 1);
	}
}

void
emusic_playlist_view_clear(Evas_Object *genlist)
{
	//clear the genlist,so we just creat a new one ...
	elm_genlist_clear(genlist);
}


