
#include "emusic_notify.h"
#include <gtk/gtk.h>


Emu_Notification* emusic_notification_new( Evas_Object *status_icon ) 
{
    E_Music_Notification *n = g_slice_new( E_Music_Notification );
    n->notification = NULL;
    n->status_icon = status_icon;
    return n;
}

void emusic_notification_free(  E_Music_Notification *n )
{
    g_slice_free (E_Music_Notification, n);
}

static void
_clear_notify (E_Music_Notification *n)
{
    if ( n->notification != NULL ) 
	notify_notification_close (n->notification, NULL);
    n->notification = NULL;
}

static void
_do_notify (E_Music_Notification *n,
		   const char *summary,
		   const char *message )
{
	NotifyNotification *notify;
	GError *error = NULL;

	g_return_if_fail (n != NULL);
	g_return_if_fail (summary != NULL);
	g_return_if_fail (message != NULL);

	lxmusic_clear_notify (n);

	notify = notify_notification_new (summary, message,
	                                  "lxmusic", NULL);
	n->notification = notify;

	notify_notification_attach_to_status_icon (notify, n->status_icon);
	notify_notification_set_urgency (notify, NOTIFY_URGENCY_NORMAL);
	notify_notification_set_timeout (notify, NOTIFY_EXPIRES_DEFAULT);

	if (!notify_notification_show (notify, &error)) {
	    g_warning ("Failed to show notification: %s", error->message);
	    g_error_free (error);
	}
}


static void send_notifcation( const char *artist, const char* title ) 
{
    if( ! GTK_WIDGET_VISIBLE(main_win) ) 
    {
	GString* notification_message = g_string_new("");
	
	if ( (artist != NULL) && (title != NULL ) ) {	
	    /* metadata available */
	    g_string_append_printf(notification_message, "<b>%s: </b><i>%s</i>", _("Artist"), artist );
	    g_string_append_printf(notification_message, "\n<b>%s: </b><i>%s</i>", _("Title"), title );
	}
	/* use filename without markup */
	else 			
	    g_string_append( notification_message, title );
	_do_notify ( lxmusic_notification, _("Now Playing:"), notification_message->str );
	g_string_free( notification_message, TRUE );
    }
}
