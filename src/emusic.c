#include <Elementary.h>
#include <glib.h>
#include "main_ui.h"
#include "backend/backend.h"
#include "emusic_config.h"

extern Em_Smart_Data  *em;

/* Functions */
	int
elm_main()
{
	if (!creat_win(em))
	{
		ERR("could not create WIN.\n");
		return 0;		
	}

	if (!emusic_callback_setup(em))
	{
		ERR("Could not creat callback.\n");
		return 0;
	}

	return 1;
}


	int
main(int argc, char **argv)
{
	em = E_MUSIC_NEW(Em_Smart_Data);


	/*     Init  Local     */
	char *curlocale=setlocale(LC_ALL, "");
	curlocale=curlocale?curlocale:setlocale(LC_ALL, "C");
	//    mylocale = curlocale?strdup(curlocale):NULL;
	bindtextdomain("e_music", LOCALEDIR);
	textdomain("e_music");

	/*      have to init it    */
	if (!eina_init())
	{
		fprintf(stderr, "ERROR: failed to init eina.");
		exit(-1);
	}
	_log_dom = eina_log_domain_register("E-music", EINA_COLOR_CYAN);
	if (_log_dom < 0)
	{
		EINA_LOG_ERR("could not register E-music log domain.");
		_log_dom = EINA_LOG_DOMAIN_GLOBAL;
	}
	eina_log_domain_level_set("E-music", 3);

	eet_init();
	emusic_config_init(NULL);
	emusic_config_load_theme();
	eina_log_domain_level_set("E-music", emusic_config->log_level);

	if (!emusic_playback_init(em))
	{
		ERR("could not init playback.\n");
		return 0;
	}

	elm_init(argc, argv);
	elm_main();

	/*  Main Loop*/
	elm_run();

	emusic_config_shutdown();
	eina_log_domain_unregister(_log_dom);	
	elm_shutdown();
	
	return 0;
}
