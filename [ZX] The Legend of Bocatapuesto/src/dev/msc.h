// msc.h
// Generado por Mojon Script Compiler de la Churrera
// Copyleft 2011 The Mojon Twins
 
// Script data & pointers
extern unsigned char mscce_0 [];
extern unsigned char mscce_1 [];
extern unsigned char mscce_2 [];
extern unsigned char mscce_3 [];
extern unsigned char mscce_4 [];
extern unsigned char mscce_5 [];
extern unsigned char mscce_6 [];
extern unsigned char mscce_7 [];
extern unsigned char mscce_8 [];
extern unsigned char msccf_0 [];
extern unsigned char msccf_1 [];
extern unsigned char msccf_2 [];
extern unsigned char msccf_3 [];
extern unsigned char msccf_4 [];
extern unsigned char msccf_5 [];
 
unsigned char *e_scripts [] = {
    0, mscce_8, 0, mscce_6, mscce_4, mscce_7, 0, 0, 0, mscce_1, mscce_0, 0, 0, 0, 0, 0, 0, 0, 0, mscce_5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
 
unsigned char *f_scripts [] = {
    0, msccf_5, 0, msccf_3, msccf_4, msccf_2, 0, 0, 0, msccf_1, msccf_0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
 
#asm
._mscce_0

    defb 0x09, 0x10, 0x03, 0x02, 0xFF, 0x20, 0x07, 0x02, 0x06, 0xFF, 0xFF

._mscce_1

    defb 0x09, 0x10, 0x03, 0x01, 0xFF, 0x20, 0x03, 0x05, 0x06, 0xFF, 0xFF

._mscce_2

    defb 0x09, 0x10, 0x01, 0x01, 0xFF, 0x20, 0x0C, 0x05, 0x06, 0xFF, 0xFF

._mscce_3

    defb 0x09, 0x10, 0x02, 0x01, 0xFF, 0x20, 0x02, 0x05, 0x06, 0xFF, 0xFF

._mscce_4

    defb 0x0C, 0x10, 0x01, 0x02, 0x10, 0x02, 0x02, 0xFF, 0x20, 0x07, 0x01, 0x06, 0xFF, 0xFF

._mscce_5

    defb 0x13, 0x10, 0x05, 0x00, 0xFF, 0x01, 0x05, 0x01, 0xE4, 0x01, 0xE4, 0x02, 0xE4, 0x03, 0xE4, 0x04
    defb 0xE4, 0x05, 0x6E, 0xFF, 0xFF

._mscce_6

    defb 0x13, 0x10, 0x06, 0x00, 0xFF, 0x01, 0x06, 0x01, 0xE4, 0x06, 0xE4, 0x07, 0xE4, 0x08, 0xE4, 0x09
    defb 0xE4, 0x0A, 0x6E, 0xFF, 0xFF

._mscce_7

    defb 0x0F, 0x10, 0x07, 0x00, 0xFF, 0x01, 0x07, 0x01, 0xE4, 0x0B, 0xE4, 0x0C, 0xE4, 0x0D, 0x6E, 0xFF
    defb 0xFF

._mscce_8

    defb 0x08, 0xF0, 0xFF, 0x51, 0x69, 0x2D, 0x70, 0x32, 0xFF, 0xFF

._msccf_0

    defb 0x0D, 0x20, 0x04, 0x03, 0x10, 0x03, 0x00, 0xFF, 0xE4, 0x0E, 0xE0, 0x07, 0x6E, 0xFF, 0x13, 0x20
    defb 0x04, 0x03, 0x10, 0x03, 0x01, 0xFF, 0xE4, 0x0F, 0xE0, 0x06, 0x20, 0x07, 0x02, 0x06, 0x01, 0x03
    defb 0x02, 0xFF, 0xFF

._msccf_1

    defb 0x0E, 0x20, 0x03, 0x05, 0xFF, 0xE0, 0x07, 0x01, 0x03, 0x01, 0x20, 0x03, 0x05, 0x06, 0xFF, 0xFF

._msccf_2

    defb 0x11, 0x10, 0x01, 0x00, 0x20, 0x0D, 0x05, 0xFF, 0xE0, 0x07, 0x20, 0x0C, 0x05, 0x06, 0x01, 0x01
    defb 0x01, 0xFF, 0xFF

._msccf_3

    defb 0x11, 0x10, 0x02, 0x00, 0x20, 0x01, 0x05, 0xFF, 0xE0, 0x07, 0x20, 0x02, 0x05, 0x06, 0x01, 0x02
    defb 0x01, 0xFF, 0xFF

._msccf_4

    defb 0x17, 0x10, 0x01, 0x01, 0x10, 0x02, 0x01, 0x20, 0x07, 0x02, 0xFF, 0xE0, 0x07, 0x01, 0x01, 0x02
    defb 0x01, 0x02, 0x02, 0x20, 0x07, 0x01, 0x06, 0xFF, 0xFF

._msccf_5

    defb 0x12, 0x10, 0x08, 0x00, 0x20, 0x07, 0x03, 0xFF, 0x01, 0x08, 0x01, 0xE0, 0x07, 0x20, 0x07, 0x02
    defb 0x06, 0xF1, 0xFF, 0xFF

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
                    case 0x6E:
                        // REDRAW
                        // Opcode: 6E
                        draw_scr_background ();
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
                    case 0xF1:
                        script_result = 1;
                        terminado = 1;
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
