cd dist\mecab\src
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %BUILD_TYPE%
nmake -f Makefile.msvc MACHINE=%BUILD_TYPE%
