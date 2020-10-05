for /f "usebackq delims=" %%a in (`C:\tools\cygwin\cygpath.exe %0`) do set FILE=%%a
C:\tools\cygwin\bin.exe --noprofile --norc -e -o pipefail %FILE%
