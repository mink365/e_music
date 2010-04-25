#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "emusic.h"

#define ELM_ADD(btn_box, icon, cb)                                   \
    ic = elm_icon_add(sd->layout);                                       \
    elm_icon_file_set(ic, emusic_config_theme_get(), icon);            			 \
    elm_icon_scale_set(ic, 0, 0);                                    \
    bt = elm_button_add(sd->layout);                                     \
    evas_object_smart_callback_add(bt, "clicked", cb, sd);           \
    elm_button_icon_set(bt, ic);                                     \
    elm_object_style_set(bt, "simple");                         	 \
    evas_object_size_hint_weight_set(bt, 0.0, 1.0);                  \
    evas_object_size_hint_align_set(bt, 0.5, 0.5);                   \
    elm_box_pack_end(btn_box, bt);                                   \
    evas_object_show(bt);                                            \
    evas_object_show(ic);                                            


int creat_win(Smart_Data *sd);
int creat_main_menu(Smart_Data *sd);
int creat_playlist(Smart_Data *sd);

void emusic_switch_pager();


#endif
