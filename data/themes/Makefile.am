

MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = edje_cc

#filesdir = $(datadir)/$(PACKAGE)
#files_DATA = e_music.edj
e_musicdir = $(pkgdatadir)/theme
e_music_DATA = e_music.edj

e_music.edj: 
	$(EDJE_CC) \
	$(top_srcdir)/data/themes/e_music.edc \
	$(top_builddir)/data/themes/e_music.edj

clean-local:
	rm -f *.edj
