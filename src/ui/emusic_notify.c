#include <Elementary.h>
#include "emusic.h"
#include "emusic_config.h"
#include "emusic_notify.h"

typedef struct _Smart_Data
{
	Evas_Object *label;
	Evas_Object *notify;
} Smart_Data;
static Smart_Data *sd;

static void _notify_bt_close(void *data, Evas_Object *obj, void *event_info);

Evas_Object *
emusic_notify_creat(Em_Smart_Data *em)
{
	Evas_Object *bx, *bt, *notify, *label;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return NULL;

	notify = elm_notify_add(em->edje);
	evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_notify_orient_set(notify, ELM_NOTIFY_ORIENT_TOP_RIGHT);
	//edje_object_part_swallow(, "box.swallow", em->notify.notify);
	sd->notify = notify;

	bx = elm_box_add(notify);
	elm_notify_content_set(notify, bx);
	elm_box_horizontal_set(bx, 1);
	evas_object_show(bx);

	label = elm_label_add(notify);
	elm_label_label_set(label, "Top Right position.");
	elm_box_pack_end(bx, label);
	evas_object_show(label);
	sd->label = label;

	bt = elm_button_add(notify);
	elm_button_label_set(bt, "Close");
	evas_object_smart_callback_add(bt, "clicked", _notify_bt_close, NULL);
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);

	return notify;
}

int
emusic_notify_show(char *label)
{
	//FIXME:edje_object_part_text_set
	elm_label_label_set(sd->label, label);
	evas_object_show(sd->notify);
}

static void
_notify_bt_close(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_hide(sd->notify);
}
