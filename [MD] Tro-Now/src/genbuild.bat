@IF ERRORLEVEL 1 PAUSE
@echo off
@echo TEST PROGRAM
@echo Compiling.............
@set GENS_HOME=C:\dev\copy\megadrive\minilib

PATH=..\..\..\bin\
REM del rom.bin
make -f makefile.gen

pause

gens.exe %GENS_HOME%\examples\megadrive\tronow\rom.bin

del *.out
del *.o
del *.lst
del *.o80
del z80_xgm.h
del *.s