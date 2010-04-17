//#include <Elementary.h>
//#include <glib.h>
#include "emusic.h"
#include "playback.h"
#include "main_ui.h"
#include "emusic_callback.h"
#include "emusic_config.h"

Smart_Data  *sd;

int _log_dom = -1;

/* Functions */
	int
elm_main()
{

	if (!creat_win(sd))
	{
		ERR("could not create WIN.\n");
		return 0;		
	}


	if (!creat_playlist(sd))
	{
		ERR("could not create Playlist Show.\n");
		return 0;		
	}


	if (!creat_main_menu(sd))
	{
		ERR("could not create MAIN_menu.\n");
		return 0;		
	}

	if (!setup_xmms2_callback(sd))
	{
		ERR("Could not creat callback.\n");
		return 0;
	}

	return 0;
}


	int
main(int argc, char **argv)
{
	sd = E_MUSIC_NEW(Smart_Data, 1);


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


	if (!playback_init(sd))
	{
		ERR("could not init playback.\n");
		return 0;
	}

	elm_init(argc, argv);

	/*  Main Loop*/
	elm_main();

	elm_run();


	emusic_config_shutdown();
	eina_log_domain_unregister(_log_dom);	
	elm_shutdown();
}
