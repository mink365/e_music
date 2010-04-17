#ifndef __EMUSIC_NOTIFY_H__
#define __EMUSIC_NOTIFY_H__

#include <libnotify/notify.h>


typedef struct _Emu_Notification Emu_Notification;

struct _Emu_Notification
{
    NotifyNotification*	notification;
    Evas_Object *status_icon;
};

void emusic_notification_free(  E_Music_Notification *n );


void emusic_send_notifcation( const char *artist, const char* title ); 

Emu_Notification* emusic_notification_new( Evas_Object *status_icon ); 

#endif
