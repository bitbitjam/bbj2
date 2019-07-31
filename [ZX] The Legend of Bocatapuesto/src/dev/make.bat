@echo off

rem GENERA MAPA VACIO
rem ..\..\zxdev\bin\map2tmx.exe 2 2 15 10 15 ..\map\mapa.tmx

rem convierte tmx en codigo...
..\..\zxdev\bin\tmxcnv.exe ..\map\mapa.tmx mapa.h enems.h
rem comprime el mapa!
..\..\zxdev\bin\tmxcompress.exe ..\map\mapa.tmx build\mapa_comprimido.bin

..\..\zxdev\bin\posterizezx.exe ..\gfx\work.png build\tileset.png
..\..\zxdev\bin\posterizezx.exe ..\gfx\font.png build\font.png
..\..\zxdev\bin\posterizezx.exe ..\gfx\sprites.png build\sprites.png

..\..\zxdev\bin\posterizezx.exe ..\gfx\loading.png build\loading.png
..\..\zxdev\bin\posterizezx.exe ..\gfx\title.png build\title.png
..\..\zxdev\bin\posterizezx.exe ..\gfx\ending.png build\ending.png

..\..\zxdev\bin\png2scr.exe build/loading.png build/screen.bin
..\..\zxdev\bin\png2scr.exe build/title.png build/title.scr
..\..\zxdev\bin\png2scr.exe build/ending.png build/ending.scr

..\..\zxdev\bin\apack.exe build/title.scr build/title.bin
..\..\zxdev\bin\apack.exe build/ending.scr build/ending.bin

..\..\zxdev\bin\gfxcnv.exe build/font.png build/tileset.png build/sprites.png tileset.h sprites.h

..\..\zxdev\bin\msc.exe ..\script\juego.spt msc.h 27

cd build

ECHO COMPILANDO!!

C:\z88dk10\bin\zcc.exe +zx -vn ../churromain.c -o juego.bin -lndos -lsplib2 -zorg=24200
..\..\..\zxdev\bin\bas2tap.exe -a10 -sjuego ..\loader.bas loader.tap
..\..\..\zxdev\bin\bin2tap -o screen.tap -a 16384 screen.bin
..\..\..\zxdev\bin\bin2tap -o main.tap -a 24200 juego.bin

copy /b loader.tap + screen.tap + main.tap bocata.tap

rem Al ultimo "Data lenght" + 24200 - 60654 (total memoria libre)
rem "C:\Program Files (x86)\Fuse\util\tzxlist.exe" main.tap

copy bocata.tap ..\..\

echo -------------------------------------------------------------------------------
echo ### LIMPIANDO ###
del loader.tap
del screen.tap
rem del main.tap
del juego.bin
echo -------------------------------------------------------------------------------
echo ### DONE ###

cd ..