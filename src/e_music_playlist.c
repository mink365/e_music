
#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>

#include "utils.h"
#include "e_music_playlist.h"
#include "xmms2_callback.h"

extern Smart_Data  *sd;

typedef struct _Listitem
{
	TrackProperties *pro;
} Listitem;

static Elm_Genlist_Item_Class itclass;

char *
_gl_label_get(const void *data, Evas_Object *obj, const char *part)
{
  	const  Listitem *tit = data;

	if (!tit || !tit->pro)
		return NULL;

      	char buf[PATH_MAX];
       	if (!strcmp(part, "elm.text"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->pro->title);
	
	}
	else if (!strcmp(part, "elm.text.sub"))
	{
		snprintf(buf, sizeof(buf), "%s", tit->pro->artist);
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
		elm_icon_file_set(ic, THEME, "icon/music");
	}
	else if (!strcmp(part, "elm.swallow.end"))
	{
		if (tit->pro->id == sd->cur_track_id)
			elm_icon_file_set(ic, THEME, "icon/playing");
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
	const int id = (int)data;

	e_music_play( id );
	/*              */      
	e_music_switch_pager();
}

void
e_music_playlist_append(Evas_Object *playlist_show, TrackProperties *properties, int list_id)
{
	Listitem *listitem;	

   	itclass.item_style     = "double_label";
      	itclass.func.label_get = _gl_label_get;
       	itclass.func.icon_get  = _gl_icon_get;
	itclass.func.state_get = NULL;
	itclass.func.del       = NULL;
	

	listitem = E_MUSIC_NEW(Listitem, 1);
	listitem->pro = properties;
	elm_genlist_item_append(playlist_show, &itclass, 
		listitem, NULL/* parent */, ELM_GENLIST_ITEM_NONE, _list_sel, (int *)list_id/* func data */ );

/* it will do _gl_label_get two , but wen we do _gl_label_get in the second time ,listitem may has been free! */
//	free(listitem);
//	listitem = NULL;
}

#if 0
e_music_playlist_clear()
{

}
#endif

