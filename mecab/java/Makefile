TARGET=MeCab
JAVAC=javac
JAVA=java
JAR=jar

OS:=$(shell uname -s)

ifeq ($(JAVA_HOME),)
ifeq ($(OS),Linux)
JAVA_HOME=/usr/lib/jvm/java-6-openjdk # XXX: make more portable
endif
ifeq ($(OS),Darwin)
JAVA_HOME=$(shell /usr/libexec/java_home)
endif
endif

ifeq ($(OS),Linux)
JNI_PLATFORM=linux
LIB_TYPE=-shared
LIB_EXT=so
LD_PATH_VAR=LD_LIBRARY_PATH
endif
ifeq ($(OS),Darwin)
JNI_PLATFORM=darwin
LIB_TYPE=-dynamiclib
LIB_EXT=dylib
LD_PATH_VAR=DYLD_LIBRARY_PATH
endif
JNI_INCLUDEDIR=$(JAVA_HOME)/include

PACKAGE=org/chasen/mecab

LIBS+=$(shell mecab-config --libs)
CPPFLAGS+=$(shell mecab-config --cflags)
INCLUDES+=-I$(JNI_INCLUDEDIR) -I$(JNI_INCLUDEDIR)/$(JNI_PLATFORM)

.PHONY: all
all:
	echo $(OS)
	$(CXX) $(CPPFLAGS) $(INCLUDES) -O3 -c -fpic $(TARGET)_wrap.cxx
	$(CXX) $(CPPFLAGS) $(LIB_TYPE) $(TARGET)_wrap.o -o lib$(TARGET).$(LIB_EXT) $(LIBS)
	$(JAVAC) $(PACKAGE)/*.java
	$(JAVAC) test.java
	$(JAR) cfv $(TARGET).jar $(PACKAGE)/*.class

.PHONY: test
test:
	env $(LD_PATH_VAR)=. $(JAVA) test

.PHONY: clean
clean:
	$(RM) -fr *.jar *.o *.$(LIB_EXT) *.class $(PACKAGE)/*.class

.PHONY: cleanall
cleanall:
	$(RM) -fr $(TARGET).java *.cxx
