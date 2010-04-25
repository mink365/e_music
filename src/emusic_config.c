
#include <Eina.h>
#include <Ecore_File.h>
#include <Elementary.h>

#include "emusic_config.h"
#include "emusic.h"
#include "utils.h"

/*  */

/*  */

Eet_Data_Descriptor *edd = NULL;


	static void
_config_descriptor_init(void)
{
	Eet_Data_Descriptor_Class eddc;

	EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Emu_Config);

	eddc.func.str_direct_alloc = NULL;
	eddc.func.str_direct_free = NULL;

	edd = eet_data_descriptor_file_new(&eddc);
	if(edd)
	{
#define EMU_CONF_ADD_BASIC(member, eet_type)                         \
		EET_DATA_DESCRIPTOR_ADD_BASIC                                        \
		(edd, Emu_Config, #member, member, eet_type)

		EMU_CONF_ADD_BASIC(config_dir, EET_T_STRING);
		EMU_CONF_ADD_BASIC(data_dir, EET_T_STRING);
		EMU_CONF_ADD_BASIC(cache_dir, EET_T_STRING);
		EMU_CONF_ADD_BASIC(cfg_file, EET_T_STRING);
		EMU_CONF_ADD_BASIC(theme, EET_T_STRING);
		EMU_CONF_ADD_BASIC(theme_file, EET_T_STRING);
		EMU_CONF_ADD_BASIC(engine, EET_T_STRING);
		EMU_CONF_ADD_BASIC(log_level, EET_T_INT);
		EMU_CONF_ADD_BASIC(log_file, EET_T_STRING);

#undef EMU_CONF_ADD_BASIC
	}
	else
	{
		ERR("EEEK! eet_data_descriptor_file_new() failed\n");
	}
}

	static Emu_Config *
_config_load(const char *filename)
{
	Eet_File *ef = NULL;
	ef = eet_open(filename, EET_FILE_MODE_READ);
	if (ef)
	{
		emusic_config = eet_data_read(ef, edd, "config");
		eet_close(ef);
	}
	if (!emusic_config)
	{
		ERR("Can't get the config data!");	
		eet_close(ef);	
	}

}

	static void
_config_save(const Emu_Config *emu_config, const char *filename)
{
	Eet_File *ef = NULL;
	Eina_Bool ret;

	ef = eet_open(filename, EET_FILE_MODE_WRITE);
	if(!ef)
	{
		ERR("Can't open config file to be writ");
	}
	else
	{
		INF("Save config file to :%s", filename);
		ret = eet_data_write
			(ef, edd, "config", emu_config, EINA_TRUE);

		if (ret == 0)
			ERR("Fail to write config file.");

	}
	eet_close(ef);
}


	static Emu_Config *
_config_new(void)
{
	Emu_Config *emu_config = NULL;

	emu_config = E_MUSIC_NEW(Emu_Config, 1);
	if(!emu_config)
	{
		ERR("Can't calloc Emu_Config");

		return NULL;
	}

	/*   Default config data    */
	char buf[4096];
	snprintf(buf, sizeof(buf), "%s/.e-music/config/",
			getenv("HOME"));  /* XDG*/
	if (!ecore_file_is_dir(buf))
		ecore_file_mkpath(buf);
	if (!ecore_file_is_dir(buf))
	{
		CRITICAL("can not create config directory: %s", buf);
		return EINA_FALSE;
	}
	emu_config->config_dir = (char *)eina_stringshare_add(buf);

	snprintf(buf, sizeof(buf), "%s/.e-music/data/",
			getenv("HOME"));  /* XDG*/
	if (!ecore_file_is_dir(buf))
		ecore_file_mkpath(buf);
	if (!ecore_file_is_dir(buf))
	{
		CRITICAL("can not create data directory: %s", buf);
		return EINA_FALSE;
	}
	emu_config->data_dir = (char *)eina_stringshare_add(buf);

	snprintf(buf, sizeof(buf), "%s/.e-music/cache/",
			getenv("HOME"));  /* XDG*/
	if (!ecore_file_is_dir(buf))
		ecore_file_mkpath(buf);
	if (!ecore_file_is_dir(buf))
	{
		CRITICAL("can not create cache directory: %s", buf);
		return EINA_FALSE;
	}
	emu_config->cache_dir = (char *)eina_stringshare_add(buf);

	snprintf(buf, sizeof(buf), "%s/.e-music/config/emusic.cfg",
			getenv("HOME"));  /* XDG*/
	emu_config->cfg_file = (char *)eina_stringshare_add(buf);	


	emu_config->theme     = "emusic";
	emu_config->engine    = "software_x11";
	emu_config->log_file  = NULL;
	emu_config->log_level = 3;

	return emu_config;
}


	static void
_config_free(const Emu_Config *emu_config)
{
	/* should we del all of string?? */
	eina_stringshare_del(emu_config->config_dir);
	eina_stringshare_del(emu_config->data_dir);
	eina_stringshare_del(emu_config->cache_dir);
	eina_stringshare_del(emu_config->cfg_file);

	free((void *)emu_config);
	emu_config = NULL;
}


	void
emusic_config_init (const char *file)
{
	char filename[4096];

	if (file)
		snprintf(filename, sizeof(filename), "%s", file);
	else
		snprintf(filename, sizeof(filename), "%s/.e-music/config/emusic.cfg",
				getenv("HOME"));  /* XDG*/

	INF("using config file: %s", filename);

	_config_descriptor_init();

	emusic_config = _config_load(filename);
	if (!emusic_config)
		emusic_config = _config_new();

}

	void
emusic_config_shutdown (void)
{

	/* save current configuration to file */
	_config_save(emusic_config, emusic_config->cfg_file);

	_config_free(emusic_config);
}

/****************************************************************************/
/*                                Theme                                     */
/****************************************************************************/

	void
emusic_config_load_theme (void)
{
	if (!emusic_config->theme)
		goto err_theme;

	emusic_config->theme_file = (char *)
		emusic_config_theme_file_get(emusic_config->theme);

	if (!emusic_config->theme_file)
		goto err_theme;

	elm_theme_overlay_add(emusic_config->theme_file);
	return;

err_theme:
	CRITICAL("couldn't load theme file!");
}

	const char *
emusic_config_theme_get()
{
	return emusic_config->theme_file;
}

	const char *
emusic_config_theme_file_get(const char *s)
{
	char tmp[4096];
	memset(tmp, 0, sizeof(tmp));

	if (!s)
		return NULL;
	if (s[0]=='/')
		snprintf(tmp, sizeof(tmp), "%s", s);

	if (!ecore_file_exists(tmp))
		snprintf(tmp, sizeof(tmp), PACKAGE_DATA_DIR "/theme/%s.edj", s);
	if (!ecore_file_exists(tmp))
		snprintf(tmp, sizeof(tmp), "%s", PACKAGE_DATA_DIR "/theme/default.edj");

	if (ecore_file_exists(tmp))
		return strdup(tmp);
	else
		return NULL;
}
