

MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = edje_cc

#filesdir = $(datadir)/$(PACKAGE)
#files_DATA = e_music.edj
emusicdir = $(pkgdatadir)/theme
emusic_DATA = emusic.edj

emusic.edj: 
	$(EDJE_CC) \
	-id $(top_srcdir)/data/themes/images/elm \
	$(top_srcdir)/data/themes/emusic.edc \
	$(top_builddir)/data/themes/emusic.edj

clean-local:
	rm -f *.edj
