# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.7 2006/05/29 15:36:08 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies
SUBDIRS = src man doc tests
confdir = $(sysconfdir)
conf_DATA = mecabrc
EXTRA_DIST = LGPL GPL BSD Makefile.train src/Makefile.msvc
EXTRA_DIR = swig example
bin_SCRIPTS = mecab-config
ACLOCAL_AMFLAGS=-I m4

dist-hook:
	for subdir in $(EXTRA_DIR); do \
	  cp -rp $$subdir $(distdir); \
	  rm -f $(distdir)/$$subdir/*~; \
	  rm -f $(distdir)/$$subdir/*.{bak,orig}; \
	done
