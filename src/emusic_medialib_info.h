#ifndef __MEIDALIB_INFO__
#define __MEDIALIB_INFO__

typedef struct _Emu_Media_Info{
	char *artist;
	char *album;
	char *title;
	char *cover_path;
	int32_t duration;
	int32_t id;
}Emu_Media_Info;

//Emu_Media_Info *emusic_mdinfo;


Emu_Media_Info *emusic_get_medialib_info(xmmsc_connection_t *conn, int id);

#endif
