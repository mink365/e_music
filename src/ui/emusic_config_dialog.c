#include <Elementary.h>
#include <emusic.h>
#include "emusic_config_dialog.h"

extern Em_Smart_Data  *em;

typedef struct _Smart_Data
{
	Evas_Object *toolbar;
	Evas_Object *fs_bt;
	Evas_Object *file_toggle;
	Evas_Object *file_entry;

	char *file;
} Smart_Data;
static Smart_Data *sd;

static void _file_chosen (void *data, Evas_Object *obj, void *event_info);
static void _scan_button_clicked_cb (void *data, Evas_Object *obj, void *event_info);
static void _config_dialog_toggle_changed_cb (void *data, Evas_Object *obj, void *event_info);
static void _flip_to (Evas_Object *obj, const char *name);
static void _toolbar_store_clicked (void *data, Evas_Object *obj, void *event_info);
static void _toolbar_shortcuts_clicked (void *data, Evas_Object *obj, void *event_info);
static void _toolbar_themes_clicked (void *data, Evas_Object *obj, void *event_info);
static void _status_store (Evas_Object *obj, Evas_Object *holder);
static void _status_shortcuts (Evas_Object *obj, Evas_Object *holder);
static void _status_themes (Evas_Object *obj, Evas_Object *holder);


int 
emusic_config_dialog_creat(Em_Smart_Data *em)
{
	Evas_Object *obj, *scan_bt, *holder;
	Evas_Object *bx, *ic, *lb, *fl;
	Elm_Toolbar_Item *item;

	sd = calloc(1, sizeof(Smart_Data));
	if (!sd) return FALSE;
	
	obj = edje_object_add(em->evas);	
  	if (!edje_object_file_set(obj, emusic_config_theme_get(), "main/config_dialog_view"))
  	{
    	int err = edje_object_load_error_get(obj);
    	const char *errmsg = edje_load_error_str(err);
    	ERR("cannot load theme '%s', group '%s': %s\n",
        	emusic_config_theme_get(),"main/config_dialog_view", errmsg);
    	evas_object_del(obj);
    	return NULL;
  	}
  	evas_object_show(obj);
	em->config_dialog = obj;
  	  	
	bx = elm_box_add(obj);
	evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	edje_object_part_swallow(obj, "config_dialog.swallow", bx);
	evas_object_show(bx);
	
	sd->toolbar = elm_toolbar_add(obj);
	elm_toolbar_homogenous_set(sd->toolbar, 0);
	evas_object_size_hint_weight_set(sd->toolbar, 0.0, 0.0);
	evas_object_size_hint_align_set(sd->toolbar, EVAS_HINT_FILL, 0.0);

	ic = elm_icon_add(obj);
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/media");
	item = elm_toolbar_item_add(sd->toolbar, ic, _("Store"), _toolbar_store_clicked, obj);
	
	ic = elm_icon_add(obj);
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/keybinding");
	item = elm_toolbar_item_add(sd->toolbar, ic, _("Shortcuts"), _toolbar_shortcuts_clicked, obj);

	ic = elm_icon_add(obj);
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/themes");
	item = elm_toolbar_item_add(sd->toolbar, ic, _("Themes"), _toolbar_themes_clicked, obj);
	
	elm_box_pack_end(bx, sd->toolbar);
   	evas_object_show(sd->toolbar);

   	
	holder = elm_table_add(obj);
	evas_object_size_hint_align_set(holder, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(holder, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_box_pack_end(bx, holder);
	evas_object_show(holder);
	evas_object_data_set(obj, "holder", holder);
	
	_status_store(obj, holder);
	_status_shortcuts(obj, holder);
	_status_themes(obj, holder);
	
	
	fl = elm_flip_add(obj);
	evas_object_size_hint_align_set(fl, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(fl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_table_pack(holder, fl, 0, 0, 1, 1);
	evas_object_show(fl);
	evas_object_data_set(obj, "flip", fl);
   	
	return 1;
}

static void 
_file_chosen (void *data, Evas_Object *obj, void *event_info)
{
	char *file = event_info;
	
	sd->file = (char *)eina_stringshare_add(file);
	
	elm_entry_entry_set(sd->file_entry, file);
	INF("File chosen: %s\n", file);			
}

static void 
_scan_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Em_Smart_Data  *em = data;
	char *file = sd->file;

	int i = emusic_media_scan(em->conn, em->main_playlist, file);

    if (i == TRUE)
	{
		emusic_notify_show(_("Scan Finished."));
	}
}

static void 
_config_dialog_toggle_changed_cb(void *data, Evas_Object *obj, void *event_info)
{	
   Eina_Bool value = elm_fileselector_button_folder_only_get(sd->fs_bt);
   elm_fileselector_button_folder_only_set(sd->fs_bt, !value);

}


static void
_flip_to(Evas_Object *obj, const char *name)
{
   Evas_Object *wid, *fl, *holder, *front, *back;
   int front_vis;
   wid        = evas_object_data_get(obj, name);
   fl         = evas_object_data_get(obj, "flip");
   holder     = evas_object_data_get(obj, "holder");
   front_vis  = elm_flip_front_get(fl);
   if (front_vis)
     {
        front = elm_flip_content_front_get(fl);
        back  = elm_flip_content_back_get(fl);
        elm_table_pack(holder, back, 0, 0, 1, 1);
        evas_object_hide(back);
        elm_table_unpack(holder, wid); // this should NOT be needed - but in evas_table/elm_table
        elm_flip_content_back_set(fl, wid);
     }
   else
     {
        front = elm_flip_content_front_get(fl);
        back  = elm_flip_content_back_get(fl);
        elm_table_pack(holder, front, 0, 0, 1, 1);
        evas_object_hide(front);
        elm_table_unpack(holder, wid); // this should NOT be needed - but in evas_table/elm_table
        elm_flip_content_front_set(fl, wid);
     }
   
   evas_object_show(wid);
   elm_flip_go(fl, ELM_FLIP_CUBE_LEFT);
}

static void 
_toolbar_store_clicked(void *data, Evas_Object *obj, void *event_info)
{
	_flip_to(data, "store");
	INF("Flip to Store. ");
}

static void 
_toolbar_shortcuts_clicked(void *data, Evas_Object *obj, void *event_info)
{
	_flip_to(data, "shortcuts");
	INF("Flip to Shortcuts. ");
}

static void 
_toolbar_themes_clicked(void *data, Evas_Object *obj, void *event_info)
{
	_flip_to(data, "themes");
	INF("Flip to Shortcuts. ");
}

static void 
_status_store(Evas_Object *obj, Evas_Object *holder)
{
	Evas_Object *vbox, *hbox, *lb, *ic, *scan_bt, *back_bt;


	vbox = elm_box_add(obj);
	evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	//evas_object_show(vbox);
	
	hbox = elm_box_add(obj);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_box_pack_end(vbox, hbox);
	evas_object_show(hbox);
   	
   
	lb = elm_label_add(obj);
	elm_label_label_set(lb, _("Last selection:"));
	elm_label_text_color_set(lb, 255, 255, 255, 255);
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	sd->file_entry = elm_entry_add(obj);
	elm_entry_line_wrap_set(sd->file_entry, EINA_FALSE);
	elm_entry_editable_set(sd->file_entry, EINA_FALSE);
	elm_box_pack_end(hbox, sd->file_entry);
	evas_object_show(sd->file_entry);
	
	/* file selector button */
	ic = elm_icon_add(obj);
	elm_icon_standard_set(ic, "file");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	sd->fs_bt = elm_fileselector_button_add(em->win);
	elm_fileselector_button_label_set(sd->fs_bt, _("Select"));
	elm_fileselector_button_icon_set(sd->fs_bt, ic);
	elm_fileselector_button_inwin_mode_set(sd->fs_bt, EINA_TRUE);
	evas_object_smart_callback_add(sd->fs_bt, "file,chosen", _file_chosen, NULL);
	elm_box_pack_end(hbox, sd->fs_bt);
	evas_object_show(sd->fs_bt);
	evas_object_show(ic);
   
		
	ic = elm_icon_add(obj);
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/songs");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	sd->file_toggle = elm_toggle_add(obj);
	evas_object_size_hint_weight_set(sd->file_toggle, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(sd->file_toggle, EVAS_HINT_FILL, 0.5);
	evas_object_smart_callback_add(sd->file_toggle, "changed", _config_dialog_toggle_changed_cb, NULL);
	elm_toggle_label_set(sd->file_toggle, "Folder or File");
	elm_toggle_icon_set(sd->file_toggle, ic);
	elm_toggle_state_set(sd->file_toggle, 1);
	elm_toggle_states_labels_set(sd->file_toggle, _("File"), _("Folder"));
	elm_box_pack_end(vbox, sd->file_toggle);
	evas_object_show(sd->file_toggle);
	evas_object_show(ic);
	
	ic = elm_icon_add(obj);                                    
	elm_icon_file_set(ic, emusic_config_theme_get(), "icon/songs"); 
	scan_bt = elm_button_add(obj);	
	evas_object_size_hint_align_set(scan_bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(scan_bt, "clicked", _scan_button_clicked_cb, em);
	elm_button_icon_set(scan_bt, ic);
	elm_button_label_set(scan_bt, "Scan");
	elm_box_pack_end(vbox, scan_bt);
	evas_object_show(scan_bt);
	evas_object_show(ic);

	
	evas_object_data_set(obj, "store", vbox);
   
	elm_table_pack(holder, vbox, 0, 0, 1, 1);

}

static void 
_status_shortcuts(Evas_Object *obj, Evas_Object *holder)
{
	Evas_Object *lb, *pd, *bx;
   
	bx = elm_box_add(obj);

	pd = elm_frame_add(obj);
	elm_object_style_set(pd, "pad_medium");
	elm_box_pack_end(bx, pd);
	evas_object_show(pd);
   
	lb = elm_label_add(obj);
	elm_label_label_set(lb,_("<hilight>Coming Soon.</>"));
	elm_label_text_color_set(lb, 255, 255, 255, 255);
	elm_frame_content_set(pd, lb);
	evas_object_show(lb);
   
	evas_object_data_set(obj, "shortcuts", bx);
   
	elm_table_pack(holder, bx, 0, 0, 1, 1);

}

static void 
_status_themes(Evas_Object *obj, Evas_Object *holder)
{
	Evas_Object *lb, *pd, *bx;
   
	bx = elm_box_add(obj);

	pd = elm_frame_add(obj);
	elm_object_style_set(pd, "pad_medium");
	elm_box_pack_end(bx, pd);
	evas_object_show(pd);
   
	lb = elm_label_add(obj);
	elm_label_label_set(lb,_("<hilight>Coming Soon.</>"));
	elm_label_text_color_set(lb, 255, 255, 255, 255);
	elm_frame_content_set(pd, lb);
	evas_object_show(lb);
   
	evas_object_data_set(obj, "themes", bx);
   
	elm_table_pack(holder, bx, 0, 0, 1, 1);

}
