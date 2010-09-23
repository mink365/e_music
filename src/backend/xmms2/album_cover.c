#include <emusic.h>
#include "album_cover.h"

static char *_on_cover_retrieve( xmmsv_t *value, const char *picture_front );

char *
emusic_album_cover_path(const char *picture_front)
{

	xmmsc_result_t *res;
	xmmsv_t *return_value;
	res = xmmsc_bindata_retrieve( em->conn, picture_front );
	xmmsc_result_wait (res);
	return_value = xmmsc_result_get_value (res);
	char *tmp_buf = _on_cover_retrieve(return_value, picture_front);

	xmmsv_unref (return_value);
//	xmmsc_result_unref (res);

	return tmp_buf;
}

static char *
_on_cover_retrieve( xmmsv_t *value, const char *picture_front )
{
	unsigned int bl;
	const unsigned char *bc;
	char *tmp_buf;
	
	if( xmmsv_get_bin( value, &bc, &bl ) )
	{

		int fd,ret;
		char buf[4096];
		snprintf(buf, sizeof(buf), "%s/.e-music/cache/%s", getenv("HOME"), picture_front);
	
		if ( ecore_file_exists(buf) != 1 )
		{
			fd = open(buf, O_WRONLY|O_CREAT, 0777);
			ret = write(fd, bc, bl);
			close(fd);

			if (ret != -1)
			{
				ERR("False to write album cover picture");
				return NULL;
			}
		}
		
		return (char *)eina_stringshare_add( buf );
	}

	return NULL;
}
