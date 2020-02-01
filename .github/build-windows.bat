REM build mecab
cd dist\mecab\src
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %BUILD_TYPE%
nmake -f Makefile.msvc MACHINE=%BUILD_TYPE%

REM build ipadic
cd ..\..\
mecab\src\mecab-dict-index.exe -d mecab-ipadic -o dic\ipadic -f EUC-JP -t UTF-8

rem Zip mecab binaries and ipadic
copy mecab-ipadic\dicrc dic\ipadic\dicrc
copy mecab-ipadic\COPYING dic\ipadic\COPYING
copy mecab-ipadic\*.def dic\ipadic\
copy mecab-ipadic\*.csv dic\ipadic\
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\src\*.dll
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\src\*.exe
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\src\*.lib
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\src\mecab.h
7z a mecab-msvc-%BUILD_TYPE%.zip .github\mecabrc
7z a mecab-msvc-%BUILD_TYPE%.zip dic
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\BSD
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\LGPL
7z a mecab-msvc-%BUILD_TYPE%.zip mecab\GPL
