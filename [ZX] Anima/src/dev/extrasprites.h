// Extrasprites.h
// Contiene sprites extra para el modo de matar enemigos de la churrera
// Sólo se incluirá (tras los sprites) si se define PLAYER_KILLS_ENEMIES
// Copyleft 2010 The Mojon Twins

// Frames extra por si se pueden eliminar los enemigos:

#if defined(PLAYER_CAN_FIRE) || defined(PLAYER_KILLS_ENEMIES) || defined(ENABLE_PURSUERS) || defined (MODE_128K)
extern unsigned char sprite_17_a []; 
#endif
extern unsigned char sprite_18_a []; 
#if defined(PLAYER_CAN_FIRE) || defined (MODE_128K)
extern unsigned char sprite_19_a [];
extern unsigned char sprite_19_b [];
#endif

extern unsigned char sprite_21_a [];
extern unsigned char sprite_21_b [];
extern unsigned char sprite_22_a [];
extern unsigned char sprite_22_b [];

#if defined(PLAYER_CAN_FIRE) || defined(PLAYER_KILLS_ENEMIES) || defined(ENABLE_PURSUERS) || defined (MODE_128K)
#asm
    ._sprite_17_a
        defb 0, 128
        defb 56, 0
        defb 117, 0
        defb 123, 0
        defb 127, 0
        defb 57, 0
        defb 0, 0
        defb 96, 0
        defb 238, 0
        defb 95, 0
        defb 31, 0
        defb 62, 0
        defb 53, 128
        defb 42, 128
        defb 20, 128
        defb 0, 192
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
 
    ._sprite_17_b
        defb 0, 3
        defb 240, 1
        defb 248, 0
        defb 236, 0
        defb 212, 0
        defb 248, 0
        defb 224, 1
        defb 24, 0
        defb 124, 0
        defb 120, 0
        defb 244, 0
        defb 168, 0
        defb 0, 1
        defb 0, 3
        defb 0, 63
        defb 0, 127
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
 
    ._sprite_17_c
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
#endasm
#endif

#asm
	._sprite_18_a
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		
	._sprite_18_b
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		
	._sprite_18_c
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
		defb 0, 255, 0, 255, 0, 255, 0, 255
#endasm

#if defined(PLAYER_CAN_FIRE) || defined (MODE_128K)
#asm	              	
	._sprite_19_a
		defb @00000000, 0
		defb @00101100, 0
		defb @00001100, 0
		defb @00100001, 0
		defb @10011010, 0
		defb @00011000, 0
		defb @01000001, 0
		defb @00000000, 0
		defb 0, 0
		defb 0, 0
		defb 0, 0
		defb 0, 0
		defb 0, 0
		defb 0, 0
		defb 0, 0
		defb 0, 0
	
	._sprite_19_b
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
#endasm
#endif

#asm	
    ._sprite_21_a
        defb 128, 63
        defb 128, 31
        defb 160, 15
        defb 112, 0
        defb 45, 128
        defb 27, 192
        defb 7, 224
        defb 15, 224
        defb 29, 192
        defb 39, 128
        defb 91, 0
        defb 3, 32
        defb 27, 192
        defb 53, 128
        defb 54, 128
        defb 119, 0
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255

    ._sprite_21_b
        defb 1, 252
        defb 1, 248
        defb 5, 192
        defb 46, 0
        defb 180, 1
        defb 216, 3
        defb 224, 7
        defb 240, 7
        defb 184, 3
        defb 228, 1
        defb 218, 0
        defb 192, 4
        defb 216, 3
        defb 172, 1
        defb 108, 1
        defb 238, 0
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255

  ._sprite_21_c
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        
    ._sprite_22_a
        defb 111, 0
        defb 116, 0
        defb 58, 128
        defb 31, 192
        defb 47, 128
        defb 112, 0
        defb 111, 0
        defb 123, 0
        defb 102, 0
        defb 112, 0
        defb 89, 0
        defb 123, 0
        defb 27, 128
        defb 3, 224
        defb 1, 252
        defb 0, 254
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255

    ._sprite_22_b
        defb 246, 0
        defb 110, 0
        defb 156, 1
        defb 184, 3
        defb 116, 1
        defb 74, 0
        defb 174, 0
        defb 254, 0
        defb 102, 0
        defb 14, 0
        defb 60, 0
        defb 110, 0
        defb 184, 1
        defb 176, 7
        defb 160, 15
        defb 0, 31
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255     
        
     ._sprite_22_c
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255
        defb 0, 255

#endasm   
