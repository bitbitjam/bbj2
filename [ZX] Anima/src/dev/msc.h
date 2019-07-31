// msc.h
// Generado por Mojon Script Compiler de la Churrera
// Copyleft 2011 The Mojon Twins
 
// Script data & pointers
extern unsigned char mscce_0 [];
extern unsigned char mscce_1 [];
extern unsigned char mscce_2 [];
extern unsigned char mscce_3 [];
extern unsigned char mscce_4 [];
extern unsigned char msccf_0 [];
extern unsigned char msccf_1 [];
extern unsigned char msccf_2 [];
extern unsigned char msccf_3 [];
 
unsigned char *e_scripts [] = {
    0, 0, 0, 0, 0, 0, mscce_2, mscce_4, mscce_0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, mscce_1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, mscce_3, 0, 0, 0
};
 
unsigned char *f_scripts [] = {
    0, 0, 0, 0, 0, 0, msccf_1, msccf_3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, msccf_0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, msccf_2, 0, 0, 0
};
 
#asm
._mscce_0

    defb 0x07, 0x10, 0x01, 0x00, 0xFF, 0xE4, 0x02, 0xFF, 0xFF

._mscce_1

    defb 0x0A, 0x10, 0x01, 0x00, 0xFF, 0x51, 0xA5, 0x14, 0xB0, 0x20, 0xFF, 0x09, 0x10, 0x01, 0x01, 0xFF
    defb 0x20, 0x0B, 0x02, 0x00, 0xFF, 0xFF

._mscce_2

    defb 0x08, 0xF0, 0xFF, 0x51, 0x6E, 0x7D, 0x8A, 0x90, 0xFF, 0xFF

._mscce_3

    defb 0x08, 0xF0, 0xFF, 0x51, 0xD2, 0x4B, 0xE0, 0x50, 0xFF, 0xFF

._mscce_4

    defb 0x08, 0xF0, 0xFF, 0x51, 0x69, 0x00, 0x70, 0x00, 0xFF, 0xFF

._msccf_0

    defb 0x13, 0x10, 0x01, 0x00, 0x20, 0x0A, 0x02, 0xFF, 0x20, 0x0B, 0x02, 0x00, 0xE0, 0x07, 0x01, 0x01
    defb 0x01, 0xE4, 0x01, 0xFF, 0xFF

._msccf_1

    defb 0x09, 0x20, 0x08, 0x09, 0xFF, 0x6D, 0x18, 0x01, 0x01, 0xFF, 0xFF

._msccf_2

    defb 0x09, 0x20, 0x0E, 0x05, 0xFF, 0x6D, 0x10, 0x01, 0x04, 0xFF, 0xFF

._msccf_3

    defb 0x0A, 0x20, 0x07, 0x00, 0xF0, 0xFF, 0x6D, 0x08, 0x01, 0x08, 0xFF, 0xFF

#endasm
 
unsigned char *script;
 
void msc_init_all (void) {
    unsigned char i;
    for (i = 0; i < MAX_FLAGS; i ++)
        flags [i] = 0;
}
 
unsigned char read_byte (void) {
    unsigned char c;
    c = script [0];
    script ++;
    return c;
}
 
unsigned char read_vbyte (void) {
    unsigned char c;
    c = read_byte ();
    if (c & 128) return flags [c & 127];
    return c;
}
 
// Ejecutamos el script apuntado por *script:
void run_script (void) {
    unsigned char terminado = 0;
    unsigned char continuar = 0;
    unsigned char x, y, n, m, c;
    unsigned char *next_script;
 
    if (script == 0)
        return; 
 
    script_something_done = 0;
 
    while (1) {
        c = read_byte ();
        if (c == 0xFF) break;
        next_script = script + c;
        terminado = continuar = 0;
        while (!terminado) {
            c = read_byte ();
            switch (c) {
                case 0x10:
                    // IF FLAG x = n
                    // Opcode: 10 x n
                    x = read_vbyte ();
                    n = read_vbyte ();
                    terminado = (flags [x] != n);
                    break;
                case 0x20:
                    // IF PLAYER_TOUCHES x, y
                    // Opcode: 20 x y
                    x = read_vbyte ();
                    y = read_vbyte ();
                    terminado = (!((player.x >> 6) >= (x << 4) - 15 && (player.x >> 6) <= (x << 4) + 15 && (player.y >> 6) >= (y << 4) - 15 && (player.y >> 6) <= (y << 4) + 15));
                    break;
                case 0xF0:
                     // IF TRUE
                     // Opcode: F0
                     break;
                case 0xFF:
                    // THEN
                    // Opcode: FF
                    terminado = 1;
                    continuar = 1;
                    script_something_done = 1;
                    break;
            }
        }
        if (continuar) {
            terminado = 0;
            while (!terminado) {
                c = read_byte ();
                switch (c) {
                    case 0x01:
                        // SET FLAG x = n
                        // Opcode: 01 x n
                        x = read_vbyte ();
                        n = read_vbyte ();
                        flags [x] = n;
                        break;
                    case 0x20:
                        // SET TILE (x, y) = n
                        // Opcode: 20 x y n
                        x = read_vbyte ();
                        y = read_vbyte ();
                        n = read_vbyte ();
                        map_buff [x + (y << 4) - y] = n;
                        map_attr [x + (y << 4) - y] = comportamiento_tiles [n];
                        draw_coloured_tile (VIEWPORT_X + x + x, VIEWPORT_Y + y + y, n);
                        break;
                    case 0x51:
                        // SET_FIRE_ZONE x1, y1, x2, y2
                        // Opcode: 51 x1 y1 x2 y2
                        fzx1 = read_byte ();
                        fzy1 = read_byte ();
                        fzx2 = read_byte ();
                        fzy2 = read_byte ();
                        f_zone_ac = 1;
                        break;
                    case 0x6D:
                        // WARP_TO n
                        // Opcode: 6D n
                        n_pant = read_vbyte ();
                        player.x = read_vbyte () << 10;
                        player.y = read_vbyte () << 10;
                        script_result = 3; terminado = 1;
                        break;
                    case 0xE0:
                        // SOUND n
                        // Opcode: E0 n
                        n = read_vbyte ();
#ifdef MODE_128K
                        wyz_play_sound (n);
#else
                        peta_el_beeper (n);
#endif
                        break;
                    case 0xE4:
                        // EXTERN n
                        // Opcode: 0xE4 n
                        n = read_vbyte ();
                        do_extern_action (n);
                        break;
                    case 0xFF:
                        terminado = 1;
                        break;
                }
            }
        }
        script = next_script;
    }
    if (script_result == 3) {
        script_result = 0;
        draw_scr ();
    }
}
