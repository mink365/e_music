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


AM_CFLAGS= @ELEMENTARY_CFLAGS@ @XMMS2_CFLAGS@ -DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)/\" \
			-DLOCALEDIR=\"\\\"$datadir/locale\\\"\" \
			-DMODEFILE=\"@MODEFILE@\"

AM_LDFLAGS = @XMMS2_LIBS@  @ELEMENTARY_LIBS@

bin_PROGRAMS = e-music

e_music_SOURCES =  emusic.c 	emusic.h \
					  main_ui.c main_ui.h \
					  emusic_config.c emusic_config.h \
					  ui/emusic_cover_art.h ui/emusic_cover_art.c \
					  ui/emusic_config_dialog.c ui/emusic_config_dialog.h \
					  ui/emusic_event_bg.c ui/emusic_event_bg.h \
					  ui/emusic_info.c ui/emusic_info.h \
				      ui/emusic_notify.c ui/emusic_notify.h \
                      ui/emusic_play_ctl.c ui/emusic_play_ctl.h \
                      ui/emusic_playlist_ctl.c ui/emusic_playlist_ctl.h \
					  ui/emusic_playlist_view.c ui/emusic_playlist_view.h \
			          ui/emusic_slider.c ui/emusic_slider.h \
					backend/xmms2/playback.c backend/xmms2/playback.h \
					backend/xmms2/collections.c backend/xmms2/collections.h \
					backend/xmms2/playlist.c backend/xmms2/playlist.h \
					backend/xmms2/album_cover.c backend/xmms2/album_cover.h \
					backend/xmms2/callback.c backend/xmms2/callback.h \
					backend/xmms2/medialib_info.h backend/xmms2/medialib_info.c \
					utils.h utils.c \
					  $(NULL)


INCLUDES = \
	-I$(top_srcdir) \
	$(NULL)


