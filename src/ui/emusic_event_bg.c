#include <Elementary.h>
#include <emusic.h>
#include "emusic_event_bg.h"

typedef struct _Smart_Data
{
  	int    mouse_act;
  	int    mouse_x;
  	int    mouse_y;
  	int    mouse_button;
} Smart_Data;
static Smart_Data *sd;

static void _bg_cb_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info);
static void _bg_cb_mouse_up(void *data, Evas *e, Evas_Object *obj, void *event_info);
static void _bg_cb_mouse_move(void *data, Evas *e, Evas_Object *obj, void *event_info);

/* local subsystem globals */
static Evas_Smart *_e_smart = NULL;
static void _widget_smart_init(void);
static void _widget_smart_add(Evas_Object *obj);
static void _e_box_smart_del(Evas_Object *obj);

int
emusic_event_bg_creat(Em_Smart_Data *em)
{
	Evas_Object *obj;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;

	obj = edje_object_add(em->evas);
	if (!edje_object_file_set(obj, emusic_config_theme_get(), "main/event_bg"))
	{
		int err = edje_object_load_error_get(obj);
		const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),"main/event_bg", errmsg);
    	evas_object_del(obj);
    	return NULL;
	}
	em->event_bg = obj;

	evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_MOVE,
				  _bg_cb_mouse_move, em);
   	evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_DOWN,
				  _bg_cb_mouse_down, em);
   	evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_UP,
				  _bg_cb_mouse_up, em);

	return 1;
}


static void
_bg_cb_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;

   sd->mouse_act = 1;
   sd->mouse_button = ev->button;
   sd->mouse_x = ev->canvas.x;
   sd->mouse_y = ev->canvas.y;
}

static void
_bg_cb_mouse_up(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Up *ev = event_info;

   sd->mouse_x = 0;
   sd->mouse_y = 0;
   sd->mouse_button = 0;
}

static void
_bg_cb_mouse_move(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Move *ev = event_info;
   int diff_y, diff_x;

   if (!sd->mouse_x)
     goto end;

   diff_x = abs(ev->cur.canvas.x - sd->mouse_x);
   diff_y = abs(ev->cur.canvas.y - sd->mouse_y);

   if (diff_y > 15 + (diff_x/2))
     {
	//edje_object_signal_emit(sd->view->bg, "e,action,hide,into", "e");
	//edje_object_signal_emit(sd->view->bg, "e,action,hide,back", "e");
	goto end;
     }

   if (sd->mouse_button == 1)
     {
	if (ev->cur.canvas.x - sd->mouse_x > 320) ////////////
	  {
	     sd->mouse_x = 0;
	     sd->mouse_y = 0;
         //FIXME:global switch ....
	     //if (sel->states->next)
	     //  evry_browse_back(sel);
		 INF("Switch Back.....");
		//emusic_switch_to_mediaplayer();
		elm_pager_content_pop(em->pager);
	  }
	if (ev->cur.canvas.x - sd->mouse_x < -320) ////////////
	  {
	     sd->mouse_x = 0;
	     sd->mouse_y = 0;
         //FIXME:global switch ....
	     //if (sel->states->next)
	     //  evry_browse_back(sel);
		 INF("Switch Next.....");
	  }
     }
   return;

 end:
   sd->mouse_x = 0;
   sd->mouse_y = 0;
}
