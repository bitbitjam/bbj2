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
	defb 2 ;noise
	defw 10,2500,552
	defb 0
SoundEffect5Data:
	defb 2 ;noise
	defw 14,1250,3900
	defb 0
