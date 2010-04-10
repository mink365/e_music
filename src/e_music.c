//#include <Elementary.h>
//#include <glib.h>
#include "e_music.h"
#include "playback.h"
#include "main_ui.h"
#include "xmms2_callback.h"

Smart_Data  *sd;


int
elm_main(int argc, char **argv)
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

	
	elm_run();	
	elm_shutdown();
	return 0;
}

int
main(int argc, char **argv)
{


//	setlocale(LC_ALL, "");
//	bindtextdomain(PACKAGE, LOCALEDIR);
//	textdomain(PACKAGE);
//	elm_init(argc, argv);

 	sd = E_MUSIC_NEW(Smart_Data, 1);

	if (!playback_init(sd))
	{
		ERR("could not init playback.\n");
		return 0;
	}

	elm_init(argc, argv);

	return elm_main(argc, argv);
}
