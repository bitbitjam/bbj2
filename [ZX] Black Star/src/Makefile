TAP_NAME=blackstr
GENERATED=player.h impact.h explosion.h flyer.h bomber.h font.h menu.h tiles.h warning.h song1.h song2.h
LIBS=lib/ucl.lib lib/ntropic.lib
CFLAGS=+zx -O3 -m -vn -Wall
LDFLAGS=-lsp1 -lballoc -lim2 -llib/ucl -llib/ntropic -zorg=25124
export PATH:=tools:$(PATH)

# only if z88dk is not system-wide installed
export PATH:=../z88dk/bin:$(PATH)
export ZCCCFG:=../z88dk/lib/config/

all:
	make -C tools
	make -C lib
	make $(TAP_NAME).tap

$(TAP_NAME).tap: loader.tap screen.tap main.tap
	cat loader.tap screen.tap main.tap > $@

main.tap: main.bin
	bin2tap -a 25124 -o $@ $<

screen.tap: loading.png.scr
	bin2tap -o screen.tap loading.png.scr

loader.tap: loader.bas
	bas2tap -q -a -s$(TAP_NAME) $< $@

main.bin: main.c int.h sprites.h beeper.h ucl.h ntropic.h $(LIBS) $(GENERATED)
	zcc $(CFLAGS) $< -o $@ $(LDFLAGS)

loading.png.scr: loading.png
	png2scr.py loading.png

player.h: player.png
	png2sprite.py -i player player.png > player.h

impact.h: impact.png
	png2sprite.py -i impact impact.png > impact.h

explosion.h: explosion.png
	png2sprite.py -i explosion explosion.png > explosion.h

flyer.h: flyer.png
	png2sprite.py -i flyer flyer.png > flyer.h

bomber.h: bomber.png
	png2sprite.py -i bomber bomber.png > bomber.h

font.h: font.fnt
	bin2h.py font.fnt font > font.h

menu.h: menu.png
	png2scr.py menu.png
	ucl < menu.png.scr > menu.bin
	bin2h.py menu.bin menu > menu.h

tiles.h: tiles.png
	png2c.py -i tiles --preferred-bg black tiles.png > tiles.h

warning.h: warning.png
	png2scr.py warning.png
	ucl < warning.png.scr > warning.bin
	bin2h.py warning.bin warning > warning.h

song1.h: song1.ntropic
	ucl < song1.ntropic > song1.bin
	bin2h.py song1.bin song1 > song1.h

song2.h: gameover.ntropic
	ucl < gameover.ntropic > song2.bin
	bin2h.py song2.bin song2 > song2.h

clean:
	rm -f *.tap *.scr zcc_opt.def *.map *.bin $(GENERATED)

cleanall: clean
	make -C tools clean
	make -C lib clean

