AUTOMAKE_OPTIONS = no-dependencies
ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES = libMeCab.la
libMeCab_la_SOURCES = MeCab_wrap.cxx

JAVAC = @JAVAC@
JAVA = @JAVA@
JAR = @JAR@

all:
	$(JAVAC) org/chasen/mecab/*.java
	$(JAR) cfv MeCab.jar org/chasen/mecab/*.class
	$(JAVAC) test.java
