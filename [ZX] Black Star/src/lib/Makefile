all: ucl.lib ntropic.lib

# only if z88dk is not system-wide installed
export PATH:=../../z88dk/bin:$(PATH)
export ZCCCFG:=../../z88dk/lib/config/

ucl.lib: ucl.asm
	echo ucl > ucl.lst
	z80asm -d -ns -nm -Mo -xucl @ucl.lst
	rm ucl.lst

ntropic.lib: ntropic.asm
	echo ntropic > ntropic.lst
	z80asm -d -ns -nm -Mo -xntropic @ntropic.lst
	rm ntropic.lst

clean:
	rm -f *.o *.lib

