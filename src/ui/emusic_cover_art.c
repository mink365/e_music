#include <emusic.h>

#include "emusic_cover_art.h"

typedef struct _Smart_Data
{
	Evas_Object *cover;
} Smart_Data;
static Smart_Data *sd;

int
emusic_cover_art_creat(Em_Smart_Data *em)
{
	Evas_Object *obj;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;

	obj = edje_object_add(em->evas);
	edje_object_file_set(obj, emusic_config_theme_get(), "widget/cover");
	em->cover_art = obj;

	return 1;
}

void emusic_cover_art_update(char *cover_path)
{
	if (!sd)
		return;

	Evas_Object *cover = sd->cover;

	evas_object_hide(cover);
	evas_object_del(cover);
	if (cover_path)
	{
		cover = elm_image_add(em->cover_art);
		elm_image_file_set(cover, cover_path, NULL);
		edje_object_part_swallow(em->cover_art, "album_cover", cover);
		evas_object_show(cover);
		sd->cover = cover;
	}
}
