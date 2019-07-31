#ifndef _HW_MD_H
#define _HW_MD_H

// tamaño pantalla
#define screen_width            320
#define screen_width_tiles      screen_width/8
#define screen_height           224
#define screen_height_tiles     screen_height/8

// controles
#define SEGA_CTRL_BUTTONS   0x0FFF
#define SEGA_CTRL_UP        0x0001
#define SEGA_CTRL_DOWN      0x0002
#define SEGA_CTRL_LEFT      0x0004
#define SEGA_CTRL_RIGHT     0x0008
#define SEGA_CTRL_B         0x0010
#define SEGA_CTRL_C         0x0020
#define SEGA_CTRL_A         0x0040
#define SEGA_CTRL_START     0x0080
#define SEGA_CTRL_Z         0x0100
#define SEGA_CTRL_Y         0x0200
#define SEGA_CTRL_X         0x0400
#define SEGA_CTRL_MODE      0x0800

// para el control del pad
extern unsigned int buttons;

// tipos de datos
typedef char  i8;
typedef short i16;
typedef long  i32;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define vu8     volatile u8
#define vu16    volatile u16
#define vu32    volatile u32

#define s8      char
#define s16     short
#define s32     long

#define uint8_t     u8

#define vs8     volatile s8

#define FALSE   0
#define TRUE    1

typedef s16 fix16;
typedef s32 fix32;

#ifdef __cplusplus
extern "C" {
#endif

// rutinas en enmsablador en hw_md.s
extern short set_sr(short new_sr);
extern short get_pad(short pad);
extern void clear_screen(void);
extern void VDP_setPalette(short *pal, int start, int count);
extern void put_str(char *str, int color, int x, int y);
extern void put_chr(char chr, int color, int x, int y);

extern void write_byte(unsigned int dst, unsigned char val);
extern void write_word(unsigned int dst, unsigned short val);
extern void write_long(unsigned int dst, unsigned int val);
extern unsigned char read_byte(unsigned int src);
extern unsigned short read_word(unsigned int src);
extern unsigned int read_long(unsigned int src);

extern void delay(int count);

extern void z80_busrequest(int flag);
extern void z80_reset(int flag);
extern void z80_memclr(void *dst, int len);
extern void z80_memcpy(void *dst, void *src, int len);

extern void VDP_loadTileData(const u32 *data, u16 index, u16 num, u8 use_dma);

#ifdef __cplusplus
}
#endif


//////////////////////////////////////////////////////////////////////////////////
// INTERRUPCIONES
void HBlankCallbackSet(void (*CallbackFunc)(void));
void VDP_setHInterrupt(u8 value);
void VDP_setHIntCounter(u8 value);

//////////////////////////////////////////////////////////////////////////////////
// VDP BGS
#define GFXCNTL 		    0xc00004
#define GFX_DMA_VRAM(adr)   ((0x4000+((adr)&0x3FFF))<<16)+(((adr)>>14)|0x80)

#define GFX_WRITE_CRAM_ADDR(adr)    ((0xC000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x00)
#define GFX_WRITE_VRAM_ADDR(adr)    ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x00)
#define GFX_WRITE_VSRAM_ADDR(adr)   ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x10)

#define TILE_ATTR_FULL(pal, pri, flipV, flipH, index)   (((flipH) << 11) + ((flipV) << 12) + ((pal) << 13) + ((pri) << 15) + (index))

#define VDP_VBLANK_FLAG         (1 << 3)

#define Z80_HALT  		        0xa11100

#define GFX_DATA_PORT           0xC00000
#define GFX_CTRL_PORT           0xC00004

#define PAL0                    0
#define PAL1                    1
#define PAL2                    2
#define PAL3                    3

#define WPLAN                   0xB000
#define HSCRL                   0xB800
#define SLIST                   0xBC00
#define APLAN                   0xC000
#define BPLAN                   0xE000

#define GET_VDPSTATUS(flag)         ((*(vu16*)(GFX_CTRL_PORT)) & (flag))

#define VDP_DMA_VRAM    0
#define VDP_DMA_CRAM    1
#define VDP_DMA_VSRAM   2
#define VDP_DMABUSY_FLAG        (1 << 1)

#define VDP_doVRamDMA(from, to, len)            \
    VDP_doDMA(VDP_DMA_VRAM, from, to, len)
#define VDP_doCRamDMA(from, to, len)            \
    VDP_doDMA(VDP_DMA_CRAM, from, to, len)
#define VDP_doVSRamDMA(from, to, len)           \
    VDP_doDMA(VDP_DMA_VSRAM, from, to, len)

#define GFX_DMA_VRAM_ADDR(adr)      ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x80)
#define GFX_DMA_CRAM_ADDR(adr)      ((0xC000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x80)
#define GFX_DMA_VSRAM_ADDR(adr)     ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x90)

#define pos_vram_font           1312

u16 temp[32768/2];

void VDP_init();
void VDP_setReg(u16 reg, u8 value);
u16 depack(const u8* src,u16* dst);
void VDP_unpackPalette(u16 *pal);
void VDP_fillTileRect(u16 plan, u16 tile, u16 x, u16 y, u16 w, u16 h);
void VDP_fillTileRectInc(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h);
void VDP_setTileMap( u16 plano, u16 tile, u16 x, u16 y );
void VDP_fillTileMap( u16 plano, const u16 *mapa, u16 x, u16 y, u16 ancho, u16 alto, u16 tile_inicial);
void VDP_fillBigTileMap( u16 plano, const u16 *mapa, int x1, int y1, int x2, int y2, int tam, int inicio_tiles, u8 pal);
void VDP_updateBigTileMap( u16 plano, const u16 *mapa, int x1, int y1, int x2, int y2, int tam, int inicio_tiles, u8 pal);
void VDP_setVerticalScroll(u16 plan, u16 cell, u16 value);
void VDP_setHorizontalScroll(u16 plan, u16 line, u16 value);
void VDP_clearPlan(u16 plan);
void VDP_drawNumber(int num, u16 plan, u16 basetile, u16 x, u16 y);
void VDP_drawText(const char *str, u16 plan, u16 basetile, u16 x, u16 y);

void VDP_doDMA(u8 location, u32 from, u16 to, u16 len);
void VDP_waitDMACompletion();
void VDP_waitVSync();
void VDP_setHilightShadow( u8 value );

///////////////////////////////////////////////////////////////////////
// SPRITES
#define SPRITE_SIZE(w, h)   ((((w) - 1) << 2) | ((h) - 1))

typedef struct
{
    s16 posx;
    s16 posy;
    u16 tile_attr;
    u8 size;
    u8 link;
} SpriteDef;

extern SpriteDef spriteDefCache[80];

void VDP_setSpriteP(u16 index, const SpriteDef *sprite);
void VDP_setSpriteDirectP(u16 index, const SpriteDef *sprite);
void VDP_setSpriteDirect(u16 index, u16 x, u16 y, u8 size, u16 tile_attr, u8 link);
void VDP_setSpritePosition(u16 index, u16 x, u16 y);
u16 VDP_getSpritePositionY(u16 index);
u16 VDP_getSpritePositionX(u16 index);
void VDP_updateSprites();
void VDP_resetSprites();

////////////////////////////////////////////////////////////////////
// PALETAS
extern const u16 palette_black[16];
extern const u16 palette_white[16];
extern const u16 palette_blue[16];
extern const u16 palette_red[16];
extern const u16 palette_green[16];

////////////////////////////////////////////////////////////////////
// FADES
#define PALETTEFADE_FRACBITS    8

#define VDPPALETTE_REDSFT           9
#define VDPPALETTE_GREENSFT         5
#define VDPPALETTE_BLUESFT          1

#define VDPPALETTE_REDMASK          0x0E00
#define VDPPALETTE_GREENMASK        0x00E0
#define VDPPALETTE_BLUEMASK         0x000E

#define GFX_READ_CRAM_ADDR(adr)     ((0x0000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x20)

u16 VDP_getPaletteColor( u16 index );
void VDP_setPaletteColor(u16 index, u16 value);

u16 VDP_initFading1(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 numframe);
u16 VDP_doStepFading1();
u16 VDP_initFading2(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 numframe);
u16 VDP_doStepFading2();
u16 VDP_initFading3(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 fromcol3, u16 tocol3, const u16 *palsrc3, const u16 *paldst3, u16 numframe);
u16 VDP_doStepFading3();
u16 VDP_initFading4(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 fromcol3, u16 tocol3, const u16 *palsrc3, const u16 *paldst3, u16 fromcol4, u16 tocol4, const u16 *palsrc4, const u16 *paldst4, u16 numframe);
u16 VDP_doStepFading4();

////////////////////////////////////////////////////////////////////
// RANDOM
#define GFX_HVCOUNTER_PORT          0xC00008
#define GET_HVCOUNTER               (*(vu16*)(GFX_HVCOUNTER_PORT))

u16 random();

////////////////////////////////////////////////////////////////////
// FIXED POINT
#define FIX16_INT_BITS              10
#define FIX16_FRAC_BITS             (16 - FIX16_INT_BITS)

#define FIX16(value)                ((fix16) ((value) * (1 << FIX16_FRAC_BITS)))
#define intToFix16(value)           ((value) << FIX16_FRAC_BITS)
#define fix16ToInt(value)           ((value) >> FIX16_FRAC_BITS)
#define fix16Int(value)             ((value) & FIX16_INT_MASK)
#define fix16Add(val1, val2)        ((val1) + (val2))
#define fix16Sub(val1, val2)        ((val1) - (val2))
#define fix16Neg(value)             (0 - (value))
#define fix16Mul(val1, val2)        (((val1) * (val2)) >> FIX16_FRAC_BITS)
#define fix16Div(val1, val2)        (((val1) << FIX16_FRAC_BITS) / (val2))

#define FIX32_INT_BITS              22
#define FIX32_FRAC_BITS             (32 - FIX32_INT_BITS)

#define FIX32(value)                ((fix32) ((value) * (1 << FIX32_FRAC_BITS)))

#define cosFix16(value)             sintab16[((value) + 256) & 1023]
#define sinFix16(value)             sintab16[(value) & 1023]

#define fix16Add(val1, val2)        ((val1) + (val2))
#define fix16ToInt(value)           ((value) >> FIX16_FRAC_BITS)

#define fix32Add(val1, val2)        ((val1) + (val2))
#define fix32ToInt(value)           ((value) >> FIX32_FRAC_BITS)
#define fix32Sub(val1, val2)        ((val1) - (val2))

#define intToFix16(value)           ((value) << FIX16_FRAC_BITS)

#define sinFix16(value)             sintab16[(value) & 1023]
#define fix16Sub(val1, val2)        ((val1) - (val2))

#ifndef _MATHS_H_
#define _MATHS_H_

#endif

u16 randbase;

////////////////////////////////////////////////////////////////////
// STRING
typedef unsigned int size_t;
typedef unsigned long uintptr_t;

// ITOA
#define INT_DIGITS 19	// enough for 64 bit integer

char* strcpy(char *to, const char *from);
char *strncpy(char *s1, const char *s2, size_t n);
void * memcpy(void *dst, const void *src, size_t len);
int memcmp( void *s1, const void *s2, size_t n);
void intToStr(long value, char *str, short minsize);
unsigned long strlen_(const char *str);
void str_cat(char *dest, const char *src);
char *itoa( int i );


////////////////////////////////////////////////////////////////////
// CD
extern unsigned int vblank_vector;
extern unsigned int gen_lvl2;
extern unsigned int Sub_Start;
extern unsigned int Sub_End;

extern void Kos_Decomp(unsigned char *src, unsigned char *dst);

char wait_cmd_ack(void);
void wait_do_cmd(char cmd);

#endif
