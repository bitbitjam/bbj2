SoundEffectsData:
	defw SoundEffect0Data
	defw SoundEffect1Data
	defw SoundEffect2Data
	defw SoundEffect3Data
	defw SoundEffect4Data
	defw SoundEffect5Data

SoundEffect0Data:
	defb 1 ;tone
	defw 10,1000,200,0,128
	defb 1 ;tone
	defw 10,1000,200,0,128
	defb 0
SoundEffect1Data:
	defb 2 ;noise
	defw 10,1000,10241
	defb 0
SoundEffect2Data:
	defb 0
SoundEffect3Data:
	defb 2 ;noise
	defw 20,700,1766
	defb 0
SoundEffect4Data:
	defb 0x02 ;noise
	defw 0xA,0X9C4,0X228
	defb 0x0
SoundEffect5Data:
	defb 0x02 ;noise
	defw 0xe,0x4e2,0xf3c
	defb 0x00
