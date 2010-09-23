#include <Elementary.h>
#include "emusic.h"
#include "emusic_config.h"
#include "emusic_notify.h"

typedef struct _Smart_Data Smart_Data;

struct _Smart_Data
{
	Evas_Object *label;
	Evas_Object *notify;
};

static void _notify_bt_close(void *data, Evas_Object *obj, void *event_info);

int
emusic_notify_creat(Em_Smart_Data *em)
{
	Evas_Object *bx, *bt, *notify;

	notify = elm_notify_add(em->edje);
	evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_notify_orient_set(notify, ELM_NOTIFY_ORIENT_TOP_RIGHT);
	//edje_object_part_swallow(, "box.swallow", em->notify.notify);
	em->notify.notify = notify;

	bx = elm_box_add(notify);
	elm_notify_content_set(em->notify.notify, bx);
	elm_box_horizontal_set(bx, 1);
	evas_object_show(bx);

	em->notify.label = elm_label_add(notify);
	elm_label_label_set(em->notify.label, "Top Right position.");
	elm_box_pack_end(bx, em->notify.label);
	evas_object_show(em->notify.label);

	bt = elm_button_add(notify);
	elm_button_label_set(bt, "Close");
	evas_object_smart_callback_add(bt, "clicked", _notify_bt_close, NULL);
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);

	return 1;
}

int
emusic_notify_show(Evas_Object *obj, char *label)
{
	//FIXME:edje_object_part_text_set
	elm_label_label_set(em->notify.label, label);
	evas_object_show(em->notify.notify);
}

static void
_notify_bt_close(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_hide(em->notify.notify);
}
