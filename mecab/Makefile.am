# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.7 2006/05/29 15:36:08 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies
SUBDIRS = src man doc tests
confdir = $(sysconfdir)
conf_DATA = mecabrc
EXTRA_DIST = LGPL GPL BSD Makefile.train src/Makefile.msvc mecab-config.bat
EXTRA_DIR = swig example
bin_SCRIPTS = mecab-config
ACLOCAL_AMFLAGS=-I m4

dist-hook:
	for subdir in $(EXTRA_DIR); do \
	  cp -rp $$subdir $(distdir); \
	  rm -f $(distdir)/$$subdir/*~; \
	  rm -f $(distdir)/$$subdir/*.{bak,orig}; \
	done

script-dist:
	for subdir in perl ruby python java; do \
	  rm -fr mecab-$${subdir}-@VERSION@; \
	  mkdir mecab-$${subdir}-@VERSION@; \
	  cp -r COPYING BSD LGPL GPL AUTHORS mecab-$${subdir}-@VERSION@; \
	  cp -r $${subdir}/* mecab-$${subdir}-@VERSION@; \
	  cp -r doc/bindings.html  mecab-$${subdir}-@VERSION@; \
	  find ./mecab-$${subdir}-@VERSION@ -type d -name CVS | xargs rm -fr; \
	  find ./mecab-$${subdir}-@VERSION@ -type d -name .svn | xargs rm -fr; \
	  rm -f mecab-$${subdir}-@VERSION@/*~; \
	  tar zcfv mecab-$${subdir}-@VERSION@.tar.gz mecab-$${subdir}-@VERSION@; \
	  rm -fr mecab-$${subdir}-@VERSION@; \
	done

script-clean:
	(cd perl; $(MAKE) clean; rm -f Makefile.old Makefile)
	(cd ruby; $(MAKE) clean; rm -f Makefile *.log)
	(cd python; python setup.py clean --all)
	(cd java; $(MAKE) clean;)
