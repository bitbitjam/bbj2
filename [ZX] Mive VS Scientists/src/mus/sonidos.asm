SoundEffectsData:
	defw SoundEffect0Data
	defw SoundEffect1Data
	defw SoundEffect2Data
	defw SoundEffect3Data
	defw SoundEffect4Data
	defw SoundEffect5Data

SoundEffect0Data:
; CONTENTO
	defb 1 ;tone
	defw 0xa,1000,0xa,0x00,0x80
	defb 0
SoundEffect1Data:
; MUERTE
	defb 2 ;noise
	defw 0x14,0x7d0,0x03
	defb 0
SoundEffect2Data:
; GANADOR
	defb 1 ;tone
	defw 0xf,0x3e8,0xc8,0x00,0x80
	defb 0
SoundEffect3Data:
; COGE LLAVES/ ITEMS
	defb 1 ;tone
	defw 0xa,0x3e8,0x309,0x5a,0x6301
	defb 0
SoundEffect4Data:
; DAÑO
	defb 1 ;tone
	defw 0xa,0x3e8,0xc8,0x2328,0x5080
	defb 0
SoundEffect5Data:
; SALTO
	defb 1 ;tone
	defw 0xa,0x3e8,0xc8,0xfa,0x4680
	defb 0
