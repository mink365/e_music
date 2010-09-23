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


AM_CFLAGS= @XMMS2_CFLAGS@ \
			@ELEMENTARY_CFLAGS@ \
			-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)/\" \
			-DLOCALEDIR=\"\\\"$datadir/locale\\\"\" \
			-DMODEFILE=\"@MODEFILE@\" \
			$(INCLUDES) \
			$(NULL)

AM_LDFLAGS = @XMMS2_LIBS@ \
			 @ELEMENTARY_LIBS@ \
			$(NULL)

sbin_PROGRAMS = libxmmsc.so

libxmmsc_so_SOURCES =  playback.c playback.h \
					collections.c collections.h \
					playlist.c playlist.h \
					album_cover.c album_cover.h \
					callback.c callback.h \
					medialib_info.h medialib_info.c \
					utils.h utils.c \
					$(NULL)

#libxmmsc_so_LDADD=../../ui/libui.so


