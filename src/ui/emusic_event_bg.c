#include <Elementary.h>
#include <emusic.h>
#include "emusic_event_bg.h"

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
	em->event_bg.obj = obj;

	evas_object_event_callback_add(em->event_bg.obj, EVAS_CALLBACK_MOUSE_MOVE,
				  _bg_cb_mouse_move, em);
   	evas_object_event_callback_add(em->event_bg.obj, EVAS_CALLBACK_MOUSE_DOWN,
				  _bg_cb_mouse_down, em);
   	evas_object_event_callback_add(em->event_bg.obj, EVAS_CALLBACK_MOUSE_UP,
				  _bg_cb_mouse_up, em);

	return 1;
}


static void
_bg_cb_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;

   em->event_bg.mouse_act = 1;
   em->event_bg.mouse_button = ev->button;
   em->event_bg.mouse_x = ev->canvas.x;
   em->event_bg.mouse_y = ev->canvas.y;
}

static void
_bg_cb_mouse_up(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Up *ev = event_info;

   em->event_bg.mouse_x = 0;
   em->event_bg.mouse_y = 0;
   em->event_bg.mouse_button = 0;
}

static void
_bg_cb_mouse_move(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Move *ev = event_info;
   int diff_y, diff_x;

   if (!em->event_bg.mouse_x)
     goto end;

   diff_x = abs(ev->cur.canvas.x - em->event_bg.mouse_x);
   diff_y = abs(ev->cur.canvas.y - em->event_bg.mouse_y);

   if (diff_y > 15 + (diff_x/2))
     {
	//edje_object_signal_emit(em->event_bg.view->bg, "e,action,hide,into", "e");
	//edje_object_signal_emit(em->event_bg.view->bg, "e,action,hide,back", "e");
	goto end;
     }

   if (em->event_bg.mouse_button == 1)
     {
	if (ev->cur.canvas.x - em->event_bg.mouse_x > 320) ////////////
	  {
	     em->event_bg.mouse_x = 0;
	     em->event_bg.mouse_y = 0;
         //FIXME:global switch ....
	     //if (sel->states->next)
	     //  evry_browse_back(sel);
		 INF("Switch Next.....");
	  }
	if (ev->cur.canvas.x - em->event_bg.mouse_x < -320) ////////////
	  {
	     em->event_bg.mouse_x = 0;
	     em->event_bg.mouse_y = 0;
         //FIXME:global switch ....
	     //if (sel->states->next)
	     //  evry_browse_back(sel);
		 INF("Switch Back.....");
		 emusic_switch_to_mediaplayer();
	  }
     }
   return;

 end:
   em->event_bg.mouse_x = 0;
   em->event_bg.mouse_y = 0;
}
