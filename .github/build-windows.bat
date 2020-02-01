REM build mecab
cd dist\mecab\src
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %BUILD_TYPE%
nmake -f Makefile.msvc MACHINE=%BUILD_TYPE%

REM build ipadic
cd ..\..\
mkdir zip\dic\ipadic
mecab\src\mecab-dict-index.exe -d mecab-ipadic -o zip\dic\ipadic -f EUC-JP -t UTF-8

rem Zip mecab binaries and ipadic
mkdir zip
copy mecab-ipadic\dicrc zip\dic\ipadic\dicrc
copy mecab-ipadic\COPYING zip\dic\ipadic\COPYING
copy mecab-ipadic\*.def zip\dic\ipadic\
copy mecab-ipadic\*.csv zip\dic\ipadic\
copy mecab\src\*.dll zip
copy mecab\src\*.exe zip
copy mecab\src\*.lib zip
copy mecab\BSD zip
copy mecab\LGPL zip
copy mecab\src\*.lib zip
copy mecab\src\mecab.h zip
copy .github\mecabrc zip
cd zip
7z a ..\mecab-msvc-%BUILD_TYPE%.zip *
