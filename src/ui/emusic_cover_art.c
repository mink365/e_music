#include <emusic.h>

#include "emusic_cover_art.h"

int
emusic_cover_art_creat(Em_Smart_Data *em)
{
	Evas_Object *obj;

	obj = edje_object_add(em->evas);
	edje_object_file_set(obj, emusic_config_theme_get(), "widget/cover");
	em->cover_art.obj = obj;

	return 1;
}

void emusic_cover_art_update(Evas_Object *obj, char *cover_path)
{
	Evas_Object *cover = em->cover_art.cover;

	evas_object_hide(cover);
	evas_object_del(cover);
	if (cover_path)
	{
		cover = elm_image_add(obj);
		elm_image_file_set(cover, cover_path, NULL);					
		edje_object_part_swallow(obj, "album_cover", cover);
		evas_object_show(cover);
	}
}
