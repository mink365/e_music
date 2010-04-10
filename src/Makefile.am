# vim:set noet ts=4:
#
# ibus-handwrite - The handwrite engine for IBus
#
# Copyright (c) 2010 microcai <microcai@fedoraproject.org>
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
			-DMODEFILE=\"@MODEFILE@\"

AM_LDFLAGS = @XMMS2_LIBS@  @ELEMENTARY_LIBS@

bin_PROGRAMS = e-music

e_music_SOURCES =  e_music.c 	e_music.h 	collections.c collections.h	\
					  main_ui.c \
					  main_ui.h \
					  playback.c \
					  playback.h \
					  xmms2_callback.c \
					  xmms2_callback.h \
					  utils.h \
					  utils.c \
					  e_music_playlist.h \
					  e_music_playlist.c \
					  $(NULL)


