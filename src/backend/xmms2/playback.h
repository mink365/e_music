#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

void emusic_playback_play ();
void emusic_playback_pause ();
void emusic_playback_next ();
void emusic_playback_prev ();
int emusic_playback_init(Em_Smart_Data *sd);

void emusic_playback_jump(int id);

void emusic_playback_seek(uint32_t time);


#endif
