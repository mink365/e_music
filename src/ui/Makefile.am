# vim:set noet ts=4:
#
# emusic - Simple music player based on Xmms2 and EFL.
#
# Copyright (c) 2010 mink365 <mink365@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

INCLUDES = \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src \
	$(NULL)

AM_CFLAGS= @ELEMENTARY_CFLAGS@ \
			@XMMS2_CFLAGS@ \
			-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)/\" \
			-DLOCALEDIR=\"\\\"$datadir/locale\\\"\" \
			-DMODEFILE=\"@MODEFILE@\" \
			$(INCLUDES) \
			$(NULL)

AM_LDFLAGS = @ELEMENTARY_LIBS@ \
			$(NULL)

sbin_PROGRAMS = libui.so

libui_so_SOURCES = emusic_cover_art.h emusic_cover_art.c \
					  emusic_config_dialog.c emusic_config_dialog.h \
					  emusic_event_bg.c emusic_event_bg.h \
					  emusic_info.c emusic_info.h \
				      emusic_notify.c emusic_notify.h \
                      emusic_play_ctl.c emusic_play_ctl.h \
                      emusic_playlist_ctl.c emusic_playlist_ctl.h \
					  emusic_playlist_view.c emusic_playlist_view.h \
			          emusic_slider.c emusic_slider.h \
					  $(NULL)

