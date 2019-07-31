#!/bin/bash

GAMENAME=anima
TOTALSCREENS=32

# Cambia "%1" por el nombre de tu güego.
rm ../$GAMENAME.tap &> /dev/null
rm -rf build &> /dev/null
mkdir -p build

echo "-----------------------------------"
echo "### REGENERANDO MAPA y SPRITES ###"

#crear mapa vacio
#map2tmx 2 2 15 10 15 ../map/mapa_vacio.tmx

# convertir tmx en ficheros cabecera de la churrera
tmxcnv ../map/mapa.tmx mapa.h enems.h
# comprimir mapa, requiere trastear el engine pero libera mucha memoria
tmxcompress ../map/mapa.tmx build/mapa_comprimido.bin &> /dev/null

posterizezx ../gfx/work.png build/tileset.png
posterizezx ../gfx/font.png build/font.png
posterizezx ../gfx/sprites.png build/sprites.png

posterizezx ../gfx/loading.png build/loading.png
posterizezx ../gfx/title.png build/title.png
posterizezx ../gfx/ending.png build/ending.png

png2scr build/loading.png build/screen.bin
png2scr build/title.png build/title.scr
png2scr build/ending.png build/ending.scr

apack build/title.scr build/title.bin
apack build/ending.scr build/ending.bin

gfxcnv build/font.png build/tileset.png build/sprites.png tileset.h sprites.h

echo "-----------------------------------"
echo "### COMPILANDO SCRIPT ###"
msc ../script/$GAMENAME.spt msc.h $TOTALSCREENS

echo " "
echo "########################"
echo "### COMPILANDO GUEGO ###"
cd build
zcc +zx -vn ../churromain.c -o $GAMENAME.bin -lndos -lsplib2 -zorg=24200
# cambia -sLOADER por el nombre que quieres que salga en Program:
bas2tap -a10 -s$GAMENAME ../loader.bas loader.tap
bin2tap screen.bin screen.tap 16384
bin2tap $GAMENAME.bin main.tap 24200
cat loader.tap screen.tap main.tap > ../../$GAMENAME.tap
cd ..
echo "### DONE ###"
TAMANO=`tzxlist build/main.tap | grep "Data length" | tail -1 | grep -o '[0-9]*'`
TOTAL=`expr $TAMANO + 24200`
ZXWEIGHT=`expr 60654 - $TOTAL`
echo "$TAMANO + 24200 = $TOTAL ( $ZXWEIGHT )"

