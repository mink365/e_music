# vim:set et ts=4:
#
# ibus-handwrite - The Ibus handwrite engine
#
# Copyright (c) 2007-2008 Huang Peng <shawn.p.huang@gmail.com>
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

# if not 1, append datestamp to the version number.
AC_INIT([e-music], [0.0.1], [http://code.google.com/p/e-music/issues/entry])
AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_CONFIG_HEADERS(config.h)

define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

m4_define([xmms2_minimum_version], [0.7])
m4_define([xmms2_client_ecore_minimum_version], [0.7])
				
# check xmms2
PKG_CHECK_MODULES(XMMS2, [
    xmms2-client >= xmms2_minimum_version
	xmms2-client-ecore >= xmms2_client_ecore_minimum_version
])

PKG_CHECK_MODULES([ELEMENTARY], elementary)

AM_CONDITIONAL(HAVE_TOMOE,[test "x$have_tomoe"="xyes"])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

AC_DEFINE(MICROCAI_WITHEMAIL,"microcai@fedoraproject.org",[author microcai and his email])
# OUTPUT files
AC_CONFIG_FILES(
Makefile
po/Makefile.in
src/Makefile
data/Makefile
data/desktop/Makefile
data/themes/Makefile
)

AC_OUTPUT
