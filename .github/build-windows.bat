REM build mecab
cd %GITHUB_WORKSPACE%\dist\mecab\src
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %BUILD_TYPE%
nmake -f Makefile.msvc MACHINE=%BUILD_TYPE%

REM build ipadic
cd %GITHUB_WORKSPACE%\dist
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
copy mecab\*.bat zip
copy mecab\BSD zip
copy mecab\LGPL zip
copy mecab\src\*.lib zip
copy mecab\src\mecab.h zip
copy %GITHUB_WORKSPACE%\.github\mecabrc zip
cd zip
7z a ..\mecab-msvc-%BUILD_TYPE%-%MECAB_VERSION%.zip *

set PATH=%GITHUB_WORKSPACE%\dist\zip\;%PATH%
cd %GITHUB_WORKSPACE%\dist\scripts
if "%BUILD_TYPE%" == "x64" (
    py -3.12-64 -m pip install -U setuptools wheel pip
    py -3.12-64 -m pip wheel .
    py -3.11-64 -m pip install -U setuptools wheel pip
    py -3.11-64 -m pip wheel .
    py -3.10-64 -m pip install -U setuptools wheel pip
    py -3.10-64 -m pip wheel .
    py -3.9-64 -m pip install -U setuptools wheel pip
    py -3.9-64 -m pip wheel .
    py -3.8-64 -m pip install -U setuptools wheel pip
    py -3.8-64 -m pip wheel .
) else if "%BUILD_TYPE%" == "x86" (
    py -3.12-32 -m pip install -U setuptools wheel pip
    py -3.12-32 -m pip wheel .
    py -3.11-32 -m pip install -U setuptools wheel pip
    py -3.11-32 -m pip wheel .
    py -3.10-32 -m pip install -U setuptools wheel pip
    py -3.10-32 -m pip wheel .
    py -3.9-32 -m pip install -U setuptools wheel pip
    py -3.9-32 -m pip wheel .
    py -3.8-32 -m pip install -U setuptools wheel pip
    py -3.8-32 -m pip wheel .
)
