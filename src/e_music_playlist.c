
#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>

#include "utils.h"
#include "e_music_playlist.h"
#include "xmms2_callback.h"

typedef struct _Listitem
{
	Elm_Genlist_Item *item;
	TrackProperties *pro;
	int id;
} Listitem;

static Elm_Genlist_Item_Class itclass;

Evas_Object *
_gl_label_get(const void *data, Evas_Object *obj, const char *part)
{
  	const Listitem *tit = data;
      	char buf[PATH_MAX];
       	if (!strcmp(part, "elm.text"))
	{
		printf("%s\n", tit->pro->title);
		printf("%d\n", tit->id);
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

	return NULL;
}

Eina_Bool
_gl_state_get(const void *data, Evas_Object *obj, const char *part)
{
	return EINA_FALSE;
}

static void
_gl_del(const void *data, Evas_Object *obj, const char *part)
{


}

static void
_list_sel(void *data, Evas_Object *obj, void *event_info)
{
//	printf("select ID\n");

  	Listitem *tit = data;
//	if(tit->id == NULL)
//		printf("ERR!!");
	printf("select ID:%s\n", tit->pro->title);

//	e_music_play( tit->id );

	/*              */      
	e_music_switch_pager();
}

void
e_music_playlist_append(Evas_Object *playlist_show, TrackProperties *properties, int list_id)
{
	static Listitem *listitem;

   	itclass.item_style     = "double_label";
      	itclass.func.label_get = _gl_label_get;
       	itclass.func.icon_get  = _gl_icon_get;
	itclass.func.state_get = _gl_state_get;
	itclass.func.del       = _gl_del;
	

	listitem = E_MUSIC_NEW(Listitem, 1);
	listitem->id = list_id;
	printf("HELLO ID:%d\n", listitem->id);
	listitem->pro = properties;
	listitem->item = elm_genlist_item_append(playlist_show, &itclass, 
		listitem, NULL/* parent */, ELM_GENLIST_ITEM_NONE, _list_sel, NULL/* func data */ );

}

#if 0
e_music_playlist_clear()
{

}
#endif

