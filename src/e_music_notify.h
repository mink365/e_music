#ifndef __E_MUSIC_NOTIFY_H__
#define __E_MUSIC_NOTIFY_H__

#include <libnotify/notify.h>


typedef struct _E_Music_Notification E_Music_Notification;

struct _E_Music_Notification
{
    NotifyNotification*	notification;
    Evas_Object *status_icon;
};

void e_music_notification_free(  E_Music_Notification *n );


void e_music_send_notifcation( const char *artist, const char* title ); 

E_Music_Notification* e_music_notification_new( Evas_Object *status_icon ); 

#endif
