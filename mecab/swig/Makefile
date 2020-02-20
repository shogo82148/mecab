SWIG = swig
PREFIX = MeCab

.PHONY: all
all: perl ruby python java csharp

.PHONY: clean
clean:
	rm -f      *.pm *.cxx *.py

.PHONY: perl
perl:	
	$(SWIG) -perl -shadow -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../perl
	mv -f $(PREFIX).pm ../perl

.PHONY: ruby
ruby:	
	$(SWIG) -ruby -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../ruby/$(PREFIX)_wrap.cpp

.PHONY: python
python:	
	$(SWIG) -python -shadow -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../python
	mv -f $(PREFIX).py ../python

.PHONY: csharp
csharp:	
	$(SWIG) -csharp -namespace MeCab -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../csharp
	mv -f *.cs ../csharp

.PHONY: java
java:
	$(SWIG) -java -package org.chasen.mecab -c++ $(PREFIX).i
	mkdir -p ../java/org/chasen/mecab
	mv -f $(PREFIX)_wrap.cxx ../java
#	rm -fr exception.java SWIGTYPE_*.java
	mv -f *.java ../java/org/chasen/mecab
