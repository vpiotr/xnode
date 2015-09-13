set path=%path%;%MINGW_HOME%\bin
mkdir bin
mingw32-make.exe -f makefile.mgw BUILD=all
pause