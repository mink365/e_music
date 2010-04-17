

#ifndef EMUSIC_CONFIG_H
#define EMUSIC_CONFIG_H

typedef struct _Emu_Config Emu_Config;

struct _Emu_Config
{
	char *config_dir;
	char *data_dir;
	char *cache_dir;
    char *cfg_file;
    char *theme;
    char *theme_file;
    char *engine;
    char *log_file;
    int log_level;
};

Emu_Config *emusic_config;

void emusic_config_load_theme (void);
const char *emusic_config_theme_get(void);
const char *emusic_config_theme_file_get(const char *s);

/*********************************/

void emusic_config_init (const char *file);
void emusic_config_shutdown(void);

#endif /* EMUSIC_CONFIG_H */
