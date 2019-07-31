#include "mini_lib.h"
#include "..\main.h"

static u8 regValues[0x13];

void aplib_decrunch(u8 *source, u8 *destination);

// no es estatico para que asi se pueda leer
SpriteDef spriteDefCache[80];

static u16 spriteNum;

// para el control del pad
unsigned int buttons = 0;

// usado para los fades
static s16 fading_palR1[64];
static s16 fading_palG1[64];
static s16 fading_palB1[64];
static s16 fading_stepR1[64];
static s16 fading_stepG1[64];
static s16 fading_stepB1[64];
static u16 fading_from1;
static u16 fading_to1;
static u16 fading_cnt1;
static s16 fading_palR2[64];
static s16 fading_palG2[64];
static s16 fading_palB2[64];
static s16 fading_stepR2[64];
static s16 fading_stepG2[64];
static s16 fading_stepB2[64];
static u16 fading_from2;
static u16 fading_to2;
static u16 fading_cnt2;
static s16 fading_palR3[64];
static s16 fading_palG3[64];
static s16 fading_palB3[64];
static s16 fading_stepR3[64];
static s16 fading_stepG3[64];
static s16 fading_stepB3[64];
static u16 fading_from3;
static u16 fading_to3;
static u16 fading_cnt3;
static s16 fading_palR4[64];
static s16 fading_palG4[64];
static s16 fading_palB4[64];
static s16 fading_stepR4[64];
static s16 fading_stepG4[64];
static s16 fading_stepB4[64];
static u16 fading_from4;
static u16 fading_to4;
static u16 fading_cnt4;


// paletas
const u16 palette_white[16] = {
    0x0000,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,

    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF,
    0x0FFF
};

const u16 palette_black[16] = {
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,

    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

const u16 palette_blue[16] = {
    0x0000,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,

    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0,
    0x0EE0
};

const u16 palette_red[16] = {
    0x0000,
    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E,

    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E,
    0x000E
};

const u16 palette_green[16] = {
    0x0000,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,

    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0,
    0x00E0
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPCIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//
// void VDP_setHInterrupt(u8 value){
// activa/desactiva la HInterrupt
//
////////////////////////////////////////////////////////
void VDP_setHInterrupt(u8 value){
    vu16 *pw;

    if (value) regValues[0x00] |= 0x10;
    else regValues[0x00] &= ~0x10;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8000 | regValues[0x00];
}


////////////////////////////////////////////////////////
//
// void VDP_setHIntCounter(u8 value){
// da valor al contador de la HInterrupt
// determina cada cuantas scanlines se ejecuta
//
////////////////////////////////////////////////////////
void VDP_setHIntCounter(u8 value){
    vu16 *pw;

    regValues[0x0A] = value;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8A00 | regValues[0x0A];
}


////////////////////////////////////////////////////////////////////
//
// void HBlankCallbackSet(void (*CallbackFunc)(void)){
// seleccionamos la rutina a ejecutar en la interrupcion HSYNC
//
////////////////////////////////////////////////////////////////////
void HBlankCallbackSet(void (*CallbackFunc)(void)){

    extern void (*hblank_vector)(void);

    hblank_vector = CallbackFunc;
}


//////////////////////////////////////////////////////////////////////////////////
// VDP BGS
////////////////////////////////////////////////////////
//
// void VDP_init()
// Pone las copias "shadow" de los registros a sus
// valores por defecto, establecidos en hw_md.s
//
////////////////////////////////////////////////////////
void VDP_init(){

    regValues[0x00] = 0x04;
    regValues[0x01] = 0x14;
    regValues[0x02] = 0x30;
    regValues[0x03] = 0x2C;
    regValues[0x04] = 0x07;
    regValues[0x05] = 0x5E;
    regValues[0x06] = 0x00;
    regValues[0x07] = 0x00;
    regValues[0x08] = 0x00;
    regValues[0x09] = 0x00;
    regValues[0x0A] = 0x00;
    regValues[0x0B] = 0x00;
    regValues[0x0C] = 0x81;
    regValues[0x0D] = 0x2E;
    regValues[0x0E] = 0x00;
    regValues[0x0F] = 0x01;
    regValues[0x10] = 0x01;
    regValues[0x11] = 0x00;
    regValues[0x12] = 0x00;
}


////////////////////////////////////////////////////////
//
// void VDP_setReg(u16 reg, u8 value){
// cambia un valor de un registro del VDP
//
////////////////////////////////////////////////////////
void VDP_setReg(u16 reg, u8 value){

    vu16 *pw;

    if (reg < 0x13) regValues[reg] = value;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8000 | (reg << 8) | value;
}

////////////////////////////////////////
//
// u16 depack(const u8* src,u16* dst)
// descomprime los tiles
//
////////////////////////////////////////
u16 depack(const u8* src,u16* dst){

	aplib_decrunch((u8*)src,(u8*)dst);

	return ((src[16]|(src[17]<<8))-1);
}


////////////////////////////////////////
//
// void unpack_palette(u16 *pal)
// descomprime la paleta
//
////////////////////////////////////////
void VDP_unpackPalette(u16 *pal){

	u16 i,col;

    for(i=0;i<16;i++)
    {
        col=temp[i];
        col=(col>>8)|(col<<8);
        *pal++=col;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_fillTileMapRect(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h){
// rellena un rectagunlo w, h del tile indicado en x, y
//
//////////////////////////////////////////////////////////////////////////////////////////
void VDP_fillTileRect(u16 plan, u16 tile, u16 x, u16 y, u16 w, u16 h){

    vu32 *plctrl;
    vu16 *pwdata;
    vu16 *pw;
    u32 addr;
    u32 planwidth;
    u16 i, j;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    planwidth = ((regValues[0x10] & 0xF) + 1) << 5;
    addr = plan + (2 * (x + (planwidth * y)));

    i = h;
    while (i--)
    {
	    *plctrl = GFX_WRITE_VRAM_ADDR(addr);

        j = w;
        while (j--) *pwdata = tile;

		addr += planwidth * 2;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_fillTileMapRectInc(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h){
// rellena un rectagunlo w, h de tiles secuenciales en x, y
//
//////////////////////////////////////////////////////////////////////////////////////////
void VDP_fillTileRectInc(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h){

    vu32 *plctrl;
    vu16 *pwdata;
    vu16 *pw;
    u32 addr;
    u32 planwidth;
    u16 tile;
    u16 i, j;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    planwidth = ((regValues[0x10] & 0xF) + 1) << 5;

    addr = plan + (2 * (x + (planwidth * y)));
    tile = basetile;

    i = h;
    while (i--)
    {
	    *plctrl = GFX_WRITE_VRAM_ADDR(addr);

        j = w;
        while (j--) *pwdata = tile++;

		addr += planwidth * 2;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VDP_setTileMap(u16 plan, u16 tile, u16 x, u16 y)
// dibujo el tile en el plano indicado, x, y
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_setTileMap( u16 plano, u16 tile, u16 x, u16 y ){

    vu32 *plctrl;
    vu16 *pwdata;

    const u32 addr = plano + (2 * (x + ((((regValues[0x10] & 0xF) + 1) << 5) * y)));

    // puntero al puerto del vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    *plctrl = GFX_WRITE_VRAM_ADDR(addr);
    *pwdata = tile;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_drawTileMap( u16 plano, const u16 *mapa, u16 x, u16 y, u16 ancho, u16 alto, u16 tile_inicial){
// dibujo el mapa de tiles en el plano indicado, x,y, ancho, alto, tile inicial y paleta
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_fillTileMap( u16 plano, const u16 *mapa, u16 x, u16 y, u16 ancho, u16 alto, u16 tile_inicial){

    vu32 *plctrl;
    vu16 *pwdata;
    vu16 *pw;
    u32 addr;
    u32 planwidth;
    u16 j;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto del vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    planwidth = ((regValues[0x10] & 0xF) + 1) << 5;
    addr = plano + (2 * (x + (planwidth * y)));

    // asigno la paleta
   // tile_inicial |= (pal & 3) << 13;

    while(alto--){

        *plctrl = GFX_WRITE_VRAM_ADDR(addr);

        j = ancho;
        while (j--){
            //*pwdata = (tile_inicial+mapa[a]);
            *pwdata = *mapa++ + tile_inicial;
        }

        //while (j--) *pwdata = tile_inicial | *mapa++;

		addr += planwidth * 2;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_fillBigTileMap u16 plano, int x1, int y1, int x2, int y2, int inicio_tiles, u8 pal){
// dibuja tiles del mapa desde x1 a x2 y de y1 a y2, de un mapa de tamaño tam, con tile inicial incio_tiles y con la paleta pal
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_fillBigTileMap( u16 plano, const u16 *mapa, int x1, int y1, int x2, int y2, int tam, int inicio_tiles, u8 pal){

	/*// forma leeeenta
	int x, y;

	for( x = x1; x <= x2; x++)
        for( y = y1; y <= y2; y++)
            VDP_setTileMap(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, inicio_tiles+grid[x][y]), x%64, y);*/

    int x, y;
    vu32 *plctrl;
    vu16 *pwdata;
    vu16 *pw;
    u32 addr;
    u32 planwidth;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    //puntero al puerto del vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    planwidth = ((regValues[0x10] & 0xF) + 1) << 5;
    addr = plano + (2 * (x1%64 + (planwidth * y1)));

    // asigno la paleta
    inicio_tiles |= (pal & 3) << 13;

    for( y = y1; y <= y2; y++){
        *plctrl = GFX_WRITE_VRAM_ADDR(addr);

        for( x = x1; x <= x2; x++)
            *pwdata = inicio_tiles+mapa[x+tam*y];

        addr += planwidth * 2;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_updateBigTileMap u16 plano, int x1, int y1, int x2, int y2, int inicio_tiles, u8 pal){
// dibuja tiles del mapa desde x1 a x2 y de y1 a y2, de un mapa de tamaño tam, con tile inicial incio_tiles y con la paleta pal
// esta funcion es mas rapido que fill por usar punteros
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_updateBigTileMap( u16 plano, const u16 *mapa, int x1, int y1, int x2, int y2, int tam, int inicio_tiles, u8 pal){

	/*// forma leeeenta
	int x, y;

	for( x = x1; x <= x2; x++)
        for( y = y1; y <= y2; y++)
            VDP_setTileMap(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, inicio_tiles+grid[x][y]), x%64, y);*/

    int y;
    vu32 *plctrl;
    vu16 *pwdata;
    vu16 *pw;
    u32 addr;
    u32 planwidth;

    const u16* src = &mapa[x1+tam*y1];

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto del vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    planwidth = ((regValues[0x10] & 0xF) + 1) << 5;
    addr = plano + (2 * (x1%64 + (planwidth * y1)));

    // asigno la paleta
    inicio_tiles |= (pal & 3) << 13;

    for( y = y1; y <= y2; y++){
        *plctrl = GFX_WRITE_VRAM_ADDR(addr);

        *pwdata = inicio_tiles+*src;

        // pasa a la siguiente linea
        src += 256;

        addr += planwidth * 2;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setVerticalScroll(u16 plan, u16 cell, u16 value){
// hace scroll del plano en vertical
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_setVerticalScroll(u16 plan, u16 cell, u16 value){

    vu16 *pw;
    vu32 *pl;
    u16 addr;

    // puntero al puerto vdp
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = (cell & 0x1F) * 4;
    if (plan == BPLAN) addr += 2;

    *pl = GFX_WRITE_VSRAM_ADDR(addr);
    *pw = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setHorizontalScroll(u16 plan, u16 line, u16 value){
// hace scroll del plano en horizontal
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_setHorizontalScroll(u16 plan, u16 line, u16 value){

    vu16 *pw;
    vu32 *pl;
    u16 addr;

    // puntero al puerto vdp
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = HSCRL + ((line & 0xFF) * 4);
    if (plan == BPLAN) addr += 2;

    *pl = GFX_WRITE_VRAM_ADDR(addr);
    *pw = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_clearPlan(u16 plan){
// limpia el plano indicado
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VDP_clearPlan(u16 plan){

    vu32 *plctrl;
    vu32 *pldata;
    u16 i;

    // puntero al puerto vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pldata = (u32 *) GFX_DATA_PORT;

    *plctrl = GFX_WRITE_VRAM_ADDR(plan);

    // unroll a bit
    i = (((regValues[0x10] & 0xF) + 1) << 5) * (((regValues[0x10] >> 4) + 1) << 5) / (2 * 8);
    while (i--)
    {
        *pldata = 0;
        *pldata = 0;
        *pldata = 0;
        *pldata = 0;
        *pldata = 0;
        *pldata = 0;
        *pldata = 0;
        *pldata = 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_drawNumber(int num, u16 plan, u16 basetile, u16 x, u16 y){
// dibuja el numero num, en el plano plan, a partir del tile basetile, en x, y
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_drawNumber(int num, u16 plan, u16 basetile, u16 x, u16 y){

    char str[16];

    intToStr(num, str, 1);

    VDP_drawText(str, plan, basetile, x, y);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_drawText(const char *str, u16 plan, u16 basetile, u16 x, u16 y){
// dibuja el texto str, en el plano plan, a partir del tile basetile, en x, y
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_drawText(const char *str, u16 plan, u16 basetile, u16 x, u16 y){

    u32 len;
    u16 data[40];
    u16 i;

    // obtiene el tamaño horizontal del plano (en celdas)
    i = ((regValues[0x10] & 0xF) + 1) << 5;
    len = strlen_(str);

    // si la cadena no cabe en el plano, la cortamos
    if (len > (i - x)) len = i - x;

    for (i = 0; i < len; i++)
        data[i] = str[i] - 32;

    VDP_fillTileMap( plan, data, x, y, len, 1, basetile);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setSpriteP(u16 index, const SpriteDef *sprite)
// establece las propiedades del sprite
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_setSpriteP(u16 index, const SpriteDef *sprite){

    SpriteDef *spriteDst;

    if (index >= 80) return;

    if (index >= spriteNum) spriteNum = index + 1;

    spriteDst = &spriteDefCache[index];

    spriteDst->posx = sprite->posx;
    spriteDst->posy = sprite->posy;
    spriteDst->tile_attr = sprite->tile_attr;
    spriteDst->size = sprite->size;
    spriteDst->link = sprite->link;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setSpriteP(u16 index, const SpriteDef *sprite)
// establece las propiedades del sprite de forma directa en vram (puede glitchear)
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_setSpriteDirectP(u16 index, const SpriteDef *sprite){
    vu32 *plctrl;
    vu16 *pwdata;
    u32 addr;
    vu16 *pw;

    if (index >= 80) return;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    addr = SLIST + (index * 8);

    // puntero al puerto vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    *plctrl = GFX_WRITE_VRAM_ADDR(addr);

    // posicion y
    *pwdata = 0x80 + sprite->posy;
    // tamaño y enlace
    *pwdata = (sprite->size << 8) | sprite->link;
    // atributos de tile
    *pwdata = sprite->tile_attr;
    // posicion x
    *pwdata = 0X80 + sprite->posx;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setSpriteDirect(u16 index, u16 x, u16 y, u8 size, u16 tile_attr, u8 link)
// lo mismo pero mas rapido
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_setSpriteDirect(u16 index, u16 x, u16 y, u8 size, u16 tile_attr, u8 link){

    vu32 *plctrl;
    vu16 *pwdata;
    u32 addr;
    vu16 *pw;

    if (index >= 80) return;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    addr = SLIST + (index * 8);

    // puntero al puerto vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    *plctrl = GFX_WRITE_VRAM_ADDR(addr);

    // posicion y
    *pwdata = 0x80 + y;
    // tamaño y enlace
    *pwdata = (size << 8) | link;
    // atributos de tile
    *pwdata = tile_attr;
    // posicion x
    *pwdata = 0X80 + x;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setSpritePosition(u16 index, u16 x, u16 y){
// estbalece la posicion de  un sprite
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_setSpritePosition(u16 index, u16 x, u16 y){

    SpriteDef *sprite;

    if (index >= 80) return;

    if (index >= spriteNum) spriteNum = index + 1;

    sprite = &spriteDefCache[index];

    sprite->posx = x;
    sprite->posy = y;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_getSpritePositionY(u16 index)
// devuelve la posicion Y de un sprite
//
//////////////////////////////////////////////////////////////////////////////////////
u16 VDP_getSpritePositionY(u16 index){

    SpriteDef *sprite;

    if (index >= 80) return 0;

    if (index >= spriteNum) spriteNum = index + 1;

    sprite = &spriteDefCache[index];

    return sprite->posy;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_getSpritePositionY(u16 index)
// devuelve la posicion Y de un sprite
//
//////////////////////////////////////////////////////////////////////////////////////
u16 VDP_getSpritePositionX(u16 index){

    SpriteDef *sprite;

    if (index >= 80) return 0;

    if (index >= spriteNum) spriteNum = index + 1;

    sprite = &spriteDefCache[index];

    return sprite->posx;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_updateSprites(){
// actualizamos los sprites
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_updateSprites(){

    vu32 *plctrl;
    vu16 *pwdata;
    vu16 *pw;

    if (spriteNum == 0) return;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    *plctrl = GFX_WRITE_VRAM_ADDR(SLIST);

    {
        SpriteDef *sprite;
        u16 i;

        sprite = &spriteDefCache[0];
        i = spriteNum;
        while(i--)
        {
            // posicion y
            *pwdata = 0x80 + sprite->posy;
            // tamaño y enlace
            *pwdata = (sprite->size << 8) | sprite->link;
            // atributos de tile
            *pwdata = sprite->tile_attr;
            // posicion x
            *pwdata = 0X80 + sprite->posx;

            // proximo sprite
            sprite++;
        }
    }

    // mp subiremos un sprite sin modificar
    spriteNum = 0;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_resetSprites(){
// reinicia la tabla cache de los sprites
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_resetSprites(){

    spriteDefCache[0].posx = -0x80;
    spriteDefCache[0].link = 0;

    // se necesita mandar el sprite null al VDP
    spriteNum = 1;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// u16 random(){
// devuelve un numero random segun GET_HVCOUNTER
//
//////////////////////////////////////////////////////////////////////////////////////
u16 random(){

    randbase ^= (randbase >> 1) ^ GET_HVCOUNTER;
    randbase ^= (randbase << 1);

    return randbase;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_getPaletteColor(u16 index) {
// devuelve un color de una paleta
//
//////////////////////////////////////////////////////////////////////////////////////
u16 VDP_getPaletteColor( u16 index ) {
    vu16 *pw;
    vu32 *pl;
    u16 addr;

    /* Point to vdp port */
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = index * 2;
    *pl = GFX_READ_CRAM_ADDR(addr);

    return *pw;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_setPaletteColor(u16 index, u16 value) {
// cambia un color de la paleta
//
//////////////////////////////////////////////////////////////////////////////////////
void VDP_setPaletteColor(u16 index, u16 value) {
    vu16 *pw;
    vu32 *pl;
    u16 addr;

    /* Point to vdp port */
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = index * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    *pw = value;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VDP_initFading1(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 numframe)
// inicia los vectores para el fade de 1 paleta
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_initFading1(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 numframe){

    u16 i;
    const u16 *src1;
    const u16 *dst1;

    // no se puede hacer un fade de 0 frames !
    if (numframe == 0) return 0;

    fading_from1 = fromcol1;
    fading_to1 = tocol1;
    fading_cnt1 = numframe;

    src1 = palsrc1;
    dst1 = paldst1;

    for(i = fading_from1; i <= fading_to1; i++)
    {
        fading_palR1[i] = ((*src1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG1[i] = ((*src1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB1[i] = ((*src1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src1++;

        fading_stepR1[i] = ((((*dst1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR1[i]) / numframe;
        fading_stepG1[i] = ((((*dst1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG1[i]) / numframe;
        fading_stepB1[i] = ((((*dst1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB1[i]) / numframe;
        dst1++;
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_doStepFading1()
// realiza el fade de 1 paleta
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_doStepFading1(){

    vu16 *pw;
    vu32 *pl;
    u16 addr;
    u16 i;

    // fin del fading ?
    if (fading_cnt1 == 0 && fading_cnt2 == 0) return 0;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = fading_from1 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from1; i <= fading_to1; i++)
    {
        u16 col;

        fading_palR1[i] += fading_stepR1[i];
        fading_palG1[i] += fading_stepG1[i];
        fading_palB1[i] += fading_stepB1[i];

        col = ((fading_palR1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt1--;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VDP_initFading2(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 numframe)
// inicia los vectores para el fade de 2 paletas
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_initFading2(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 numframe){

    u16 i;
    const u16 *src1;
    const u16 *dst1;

    const u16 *src2;
    const u16 *dst2;

    // no se puede hacer un fade de 0 frames !
    if (numframe == 0) return 0;

    fading_from1 = fromcol1;
    fading_to1 = tocol1;
    fading_cnt1 = numframe;

    src1 = palsrc1;
    dst1 = paldst1;

    for(i = fading_from1; i <= fading_to1; i++)
    {
        fading_palR1[i] = ((*src1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG1[i] = ((*src1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB1[i] = ((*src1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src1++;

        fading_stepR1[i] = ((((*dst1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR1[i]) / numframe;
        fading_stepG1[i] = ((((*dst1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG1[i]) / numframe;
        fading_stepB1[i] = ((((*dst1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB1[i]) / numframe;
        dst1++;
    }

    fading_from2 = fromcol2;
    fading_to2 = tocol2;
    fading_cnt2 = numframe;

    src2 = palsrc2;
    dst2 = paldst2;

    for(i = fading_from2; i <= fading_to2; i++)
    {
        fading_palR2[i] = ((*src2 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG2[i] = ((*src2 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB2[i] = ((*src2 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src2++;

        fading_stepR2[i] = ((((*dst2 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR2[i]) / numframe;
        fading_stepG2[i] = ((((*dst2 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG2[i]) / numframe;
        fading_stepB2[i] = ((((*dst2 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB2[i]) / numframe;
        dst2++;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_doStepFading()
// realiza el fade de 2 paletas
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_doStepFading2(){

    vu16 *pw;
    vu32 *pl;
    u16 addr;
    u16 i;

    // fin del fading ?
    if (fading_cnt1 == 0 && fading_cnt2 == 0) return 0;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = fading_from1 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from1; i <= fading_to1; i++)
    {
        u16 col;

        fading_palR1[i] += fading_stepR1[i];
        fading_palG1[i] += fading_stepG1[i];
        fading_palB1[i] += fading_stepB1[i];

        col = ((fading_palR1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt1--;

    // paleta 2
    addr = fading_from2 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from2; i <= fading_to2; i++)
    {
        u16 col;

        fading_palR2[i] += fading_stepR2[i];
        fading_palG2[i] += fading_stepG2[i];
        fading_palB2[i] += fading_stepB2[i];

        col = ((fading_palR2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt2--;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VDP_initFading3(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 numframe)
// inicia los vectores para el fade de 3 paletas
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_initFading3(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 fromcol3, u16 tocol3, const u16 *palsrc3, const u16 *paldst3, u16 numframe){

    u16 i;
    const u16 *src1;
    const u16 *dst1;

    const u16 *src2;
    const u16 *dst2;

    const u16 *src3;
    const u16 *dst3;

    // no se puede hacer un fade de 0 frames !
    if (numframe == 0) return 0;

    fading_from1 = fromcol1;
    fading_to1 = tocol1;
    fading_cnt1 = numframe;

    src1 = palsrc1;
    dst1 = paldst1;

    for(i = fading_from1; i <= fading_to1; i++)
    {
        fading_palR1[i] = ((*src1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG1[i] = ((*src1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB1[i] = ((*src1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src1++;

        fading_stepR1[i] = ((((*dst1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR1[i]) / numframe;
        fading_stepG1[i] = ((((*dst1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG1[i]) / numframe;
        fading_stepB1[i] = ((((*dst1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB1[i]) / numframe;
        dst1++;
    }

    fading_from2 = fromcol2;
    fading_to2 = tocol2;
    fading_cnt2 = numframe;

    src2 = palsrc2;
    dst2 = paldst2;

    for(i = fading_from2; i <= fading_to2; i++)
    {
        fading_palR2[i] = ((*src2 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG2[i] = ((*src2 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB2[i] = ((*src2 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src2++;

        fading_stepR2[i] = ((((*dst2 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR2[i]) / numframe;
        fading_stepG2[i] = ((((*dst2 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG2[i]) / numframe;
        fading_stepB2[i] = ((((*dst2 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB2[i]) / numframe;
        dst2++;
    }

    fading_from3 = fromcol3;
    fading_to3 = tocol3;
    fading_cnt3 = numframe;

    src3 = palsrc3;
    dst3 = paldst3;

    for(i = fading_from3; i <= fading_to3; i++)
    {
        fading_palR3[i] = ((*src3 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG3[i] = ((*src3 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB3[i] = ((*src3 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src3++;

        fading_stepR3[i] = ((((*dst3 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR3[i]) / numframe;
        fading_stepG3[i] = ((((*dst3 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG3[i]) / numframe;
        fading_stepB3[i] = ((((*dst3 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB3[i]) / numframe;
        dst3++;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_doStepFading()
// realiza el fade de paletas
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_doStepFading3(){

    vu16 *pw;
    vu32 *pl;
    u16 addr;
    u16 i;

    // fin del fading ?
    if (fading_cnt1 == 0 && fading_cnt2 == 0 && fading_cnt3 == 0) return 0;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = fading_from1 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from1; i <= fading_to1; i++)
    {
        u16 col;

        fading_palR1[i] += fading_stepR1[i];
        fading_palG1[i] += fading_stepG1[i];
        fading_palB1[i] += fading_stepB1[i];

        col = ((fading_palR1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt1--;

    // paleta 2
    addr = fading_from2 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from2; i <= fading_to2; i++)
    {
        u16 col;

        fading_palR2[i] += fading_stepR2[i];
        fading_palG2[i] += fading_stepG2[i];
        fading_palB2[i] += fading_stepB2[i];

        col = ((fading_palR2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt2--;

     // paleta 3
    addr = fading_from3 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from3; i <= fading_to3; i++)
    {
        u16 col;

        fading_palR3[i] += fading_stepR3[i];
        fading_palG3[i] += fading_stepG3[i];
        fading_palB3[i] += fading_stepB3[i];

        col = ((fading_palR3[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG3[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB3[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt3--;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VDP_initFading3(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 numframe)
// inicia los vectores para el fade de 3 paletas
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_initFading4(u16 fromcol1, u16 tocol1, const u16 *palsrc1, const u16 *paldst1, u16 fromcol2, u16 tocol2, const u16 *palsrc2, const u16 *paldst2, u16 fromcol3, u16 tocol3, const u16 *palsrc3, const u16 *paldst3, u16 fromcol4, u16 tocol4, const u16 *palsrc4, const u16 *paldst4, u16 numframe){

    u16 i;
    const u16 *src1;
    const u16 *dst1;

    const u16 *src2;
    const u16 *dst2;

    const u16 *src3;
    const u16 *dst3;

    const u16 *src4;
    const u16 *dst4;

    // no se puede hacer un fade de 0 frames !
    if (numframe == 0) return 0;

    fading_from1 = fromcol1;
    fading_to1 = tocol1;
    fading_cnt1 = numframe;

    src1 = palsrc1;
    dst1 = paldst1;

    for(i = fading_from1; i <= fading_to1; i++)
    {
        fading_palR1[i] = ((*src1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG1[i] = ((*src1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB1[i] = ((*src1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src1++;

        fading_stepR1[i] = ((((*dst1 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR1[i]) / numframe;
        fading_stepG1[i] = ((((*dst1 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG1[i]) / numframe;
        fading_stepB1[i] = ((((*dst1 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB1[i]) / numframe;
        dst1++;
    }

    fading_from2 = fromcol2;
    fading_to2 = tocol2;
    fading_cnt2 = numframe;

    src2 = palsrc2;
    dst2 = paldst2;

    for(i = fading_from2; i <= fading_to2; i++)
    {
        fading_palR2[i] = ((*src2 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG2[i] = ((*src2 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB2[i] = ((*src2 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src2++;

        fading_stepR2[i] = ((((*dst2 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR2[i]) / numframe;
        fading_stepG2[i] = ((((*dst2 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG2[i]) / numframe;
        fading_stepB2[i] = ((((*dst2 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB2[i]) / numframe;
        dst2++;
    }

    fading_from3 = fromcol3;
    fading_to3 = tocol3;
    fading_cnt3 = numframe;

    src3 = palsrc3;
    dst3 = paldst3;

    for(i = fading_from3; i <= fading_to3; i++)
    {
        fading_palR3[i] = ((*src3 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG3[i] = ((*src3 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB3[i] = ((*src3 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src3++;

        fading_stepR3[i] = ((((*dst3 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR3[i]) / numframe;
        fading_stepG3[i] = ((((*dst3 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG3[i]) / numframe;
        fading_stepB3[i] = ((((*dst3 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB3[i]) / numframe;
        dst3++;
    }

    fading_from4 = fromcol4;
    fading_to4 = tocol4;
    fading_cnt4 = numframe;

    src4 = palsrc4;
    dst4 = paldst4;

    for(i = fading_from4; i <= fading_to4; i++)
    {
        fading_palR4[i] = ((*src4 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS;
        fading_palG4[i] = ((*src4 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS;
        fading_palB4[i] = ((*src4 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS;
        src4++;

        fading_stepR4[i] = ((((*dst4 & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT) << PALETTEFADE_FRACBITS) - fading_palR4[i]) / numframe;
        fading_stepG4[i] = ((((*dst4 & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT) << PALETTEFADE_FRACBITS) - fading_palG4[i]) / numframe;
        fading_stepB4[i] = ((((*dst4 & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT) << PALETTEFADE_FRACBITS) - fading_palB4[i]) / numframe;
        dst4++;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// u16 VDP_doStepFading()
// realiza el fade de paletas
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u16 VDP_doStepFading4(){

    vu16 *pw;
    vu32 *pl;
    u16 addr;
    u16 i;

    // fin del fading ?
    if (fading_cnt1 == 0 && fading_cnt2 == 0 && fading_cnt3 == 0) return 0;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    // puntero al puerto vdp
    pw = (u16 *) GFX_DATA_PORT;
    pl = (u32 *) GFX_CTRL_PORT;

    addr = fading_from1 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from1; i <= fading_to1; i++)
    {
        u16 col;

        fading_palR1[i] += fading_stepR1[i];
        fading_palG1[i] += fading_stepG1[i];
        fading_palB1[i] += fading_stepB1[i];

        col = ((fading_palR1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB1[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt1--;

    // paleta 2
    addr = fading_from2 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from2; i <= fading_to2; i++)
    {
        u16 col;

        fading_palR2[i] += fading_stepR2[i];
        fading_palG2[i] += fading_stepG2[i];
        fading_palB2[i] += fading_stepB2[i];

        col = ((fading_palR2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB2[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt2--;

     // paleta 3
    addr = fading_from3 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from3; i <= fading_to3; i++)
    {
        u16 col;

        fading_palR3[i] += fading_stepR3[i];
        fading_palG3[i] += fading_stepG3[i];
        fading_palB3[i] += fading_stepB3[i];

        col = ((fading_palR3[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG3[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB3[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt3--;

    // paleta 4
    addr = fading_from4 * 2;
    *pl = GFX_WRITE_CRAM_ADDR(addr);

    for(i = fading_from4; i <= fading_to4; i++)
    {
        u16 col;

        fading_palR4[i] += fading_stepR4[i];
        fading_palG4[i] += fading_stepG4[i];
        fading_palB4[i] += fading_stepB4[i];

        col = ((fading_palR4[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_REDSFT) & VDPPALETTE_REDMASK;
        col |= ((fading_palG4[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_GREENSFT) & VDPPALETTE_GREENMASK;
        col |= ((fading_palB4[i] >> PALETTEFADE_FRACBITS) << VDPPALETTE_BLUESFT) & VDPPALETTE_BLUEMASK;
        *pw = col;
    }

    fading_cnt4--;

    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_doDMA(u8 location, u32 from, u16 to, u16 len){
// realiza el DMA
//
/////////////////////////////////////////////////////////////////////////////////////////////
void VDP_doDMA(u8 location, u32 from, u16 to, u16 len){

    vu16 *pw;
    vu32 *pl;
    u32 newlen;
    u32 banklimit;

    // DMA trabaja en banco de 128 KB
    banklimit = 0x20000 - (from & 0x1FFFF);

    // si excedemos el tamaño del banco
    if (len > banklimit)
    {
        // primero hacemos la transferencia del segundo banco
        VDP_doDMA(location, from + banklimit, to + banklimit, len - banklimit);
        newlen = banklimit;
    }
    // si esta bien, usamos len normal
    else newlen = len;

    regValues[0x0F] = 2;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8F00 | 2;

    pw = (u16 *) GFX_CTRL_PORT;

    // Longitud del DMA (en palabras)
    newlen >>= 1;
    *pw = 0x9300 + (newlen & 0xff);
    newlen >>= 8;
    *pw = 0x9400 + (newlen & 0xff);

    // Direccion del DMA
    from >>= 1;
    *pw = 0x9500 + (from & 0xff);
    from >>= 8;
    *pw = 0x9600 + (from & 0xff);
    from >>= 8;
    *pw = 0x9700 + (from & 0x7f);

    // Activamos el DMA
    pl = (u32 *) GFX_CTRL_PORT;
    switch(location)
    {
        case VDP_DMA_VRAM:
            *pl = GFX_DMA_VRAM_ADDR(to);
            break;

        case VDP_DMA_CRAM:
            *pl = GFX_DMA_CRAM_ADDR(to);
            break;

        case VDP_DMA_VSRAM:
            *pl = GFX_DMA_VSRAM_ADDR(to);
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// void VDP_waitDMACompletion(){
// espera a que se complete el DMA anterior
//
/////////////////////////////////////////////////////////////////////////////////////////////
void VDP_waitDMACompletion(){
    while(GET_VDPSTATUS(VDP_DMABUSY_FLAG));
}

////////////////////////////////////////////////////////
//
// void VDP_waitVSync()
// espera a la interrupcion VSYNC
//
////////////////////////////////////////////////////////
void VDP_waitVSync(){

    /*vu16 *pw;
    u16 vdp_state;

    vdp_state = VDP_VBLANK_FLAG;
    pw = (u16 *) GFX_CTRL_PORT;

    while (vdp_state & VDP_VBLANK_FLAG) vdp_state = *pw;
    while (!(vdp_state & VDP_VBLANK_FLAG)) vdp_state = *pw;*/

    // lo mismo pero en ensamblador
    delay(1);

    // actualizamos el psg
    //psgFxFrame();

    // actualiza los sprites
    VDP_updateSprites();
}

////////////////////////////////////////////////////////
//
// void VDP_setHilightShadow(u8 value)
// activa el modo shadow
//
////////////////////////////////////////////////////////
void VDP_setHilightShadow(u8 value){

    vu16 *pw;

    if (value) regValues[0x0C] |= 0x09;
    else regValues[0x0C] = 0x81;

    pw = (u16 *) GFX_CTRL_PORT;
    *pw = 0x8C00 | regValues[0x0C];
}


////////////////////////////////////////////////////////////////
//
// STRING STRING STRING
//
////////////////////////////////////////////////////////////////
static unsigned long uintToStr_(unsigned long value, char *str, short minsize, short maxsize);

char* strcpy(char *to, const char *from){

    const char *src;
    char *dst;

    src = from;
    dst = to;

    while ((*dst++ = *src++));

    return to;
}

char *strncpy(char *s1, const char *s2, size_t n){
        char *s = s1;

    while (n > 0 && *s2 != '\0') {
        *s++ = *s2++;
        --n;
    }

    while (n > 0) {
        *s++ = '\0';
        --n;
    }

    return s1;
}

unsigned long strlen(const char *str){

    const char *src;

    src = str;

    while (*src++);

    return (src - str) - 1;
}

static unsigned long uintToStr_(unsigned long value, char *str, short minsize, short maxsize){

    unsigned long res;
    short cnt;
    short left;
    char data[16];
    char *src;
    char *dst;

    src = &data[16];
    res = value;
    left = minsize;

    cnt = 0;
    while (res)
    {
        *--src = '0' + (res % 10);
        res /= 10;
        cnt++;
        left--;
    }
    while (left > 0)
    {
        *--src = '0';
        cnt++;
        left--;
    }
    if (cnt > maxsize) cnt = maxsize;

    dst = str;
    while(cnt--) *dst++ = *src++;
    *dst = 0;

    return strlen(str);
}

void intToStr(long value, char *str, short minsize){

    unsigned long v;
    char *dst = str;

    if (value < 0)
    {
        v = -value;
        *dst++ = '-';
    }
    else v = value;

    uintToStr_(v, dst, minsize, 16);
}

//////////////////////////////////////////////////////////////////////////
//
// int memcmp( void *s1, const void *s2, size_t n)
// compara dos variables de tamaño n
//
//////////////////////////////////////////////////////////////////////////
int memcmp( void *s1, const void *s2, size_t n ){

    if (n != 0) {
		unsigned char *p1 = (unsigned char *)s1,
			      *p2 = (unsigned char *)s2;
		do {
			if (*p1++ != *p2++)
				return (*--p1 - *--p2);
		} while (--n != 0);
	}
	return (0);
}

////////////////////////////////////////////////////////////////
//
// void * memcpy(void *dst, const void *src, size_t len)
// copia un bloque de memoria src en dst de tamaño len
//
////////////////////////////////////////////////////////////////
void * memcpy(void *dst, const void *src, size_t len){

    size_t i;

    if ((uintptr_t)dst % sizeof(long) == 0 &&
    (uintptr_t)src % sizeof(long) == 0 &&
    len % sizeof(long) == 0) {

        long *d = dst;
        const long *s = src;

        for (i=0; i<len/sizeof(long); i++)
            d[i] = s[i];
    }
    else{
        char *d = dst;
        const char *s = src;

        for (i=0; i<len; i++)
            d[i] = s[i];
    }

    return dst;
}

////////////////////////////////////////////////////////////////
//
// u32 strlen_(const char *str){
// devuelve la longitud de la cadena
//
////////////////////////////////////////////////////////////////
unsigned long strlen_(const char *str){

    const char *src;

    src = str;

    while (*src++);

    return (src - str) - 1;
}

////////////////////////////////////////////////////////////////////////////////////
//
//  void str_cat(char *dest, const char *src){
//  copia una cadena origen en destino
//
////////////////////////////////////////////////////////////////////////////////////
void str_cat(char *dest, const char *src){

    while (*dest!= '\0')
        *dest++ ;
    do
    {
        *dest++ = *src++;
    }
    while (*src != '\0') ;
}


////////////////////////////////////////////////////////////////////////////////////
//
//  char *itoa(i)
//  convierte un entero en string
//
////////////////////////////////////////////////////////////////////////////////////
char *itoa( int i ){

    /* Room for INT_DIGITS digits, - and '\0' */
    static char buf[INT_DIGITS + 2];
    char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */

    if(i >= 0){
        do{
            *--p = '0' + (i % 10);
            i /= 10;
        }while (i != 0);

        return p;
    }
    else{			/* i < 0 */
        do{
            *--p = '0' - (i % 10);
            i /= 10;
        }while (i != 0);

        *--p = '-';
    }

    return p;
}


////////////////////////////////////////////////////////////////
//
// CD CD CD
//
////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//  char wait_cmd_ack(void){
//  espera al byte de confirmacion en el puerto comm secundario
//
//////////////////////////////////////////////////////////////////////////
char wait_cmd_ack(void){
    char ack = 0;

    while (!ack)
        ack = read_byte(0xA1200F);  // espera al byte de confirmacion en el puerto comm secundario

    return ack;
}


//////////////////////////////////////////////////////////////////////////
//
//  void wait_do_cmd(char cmd)
//  espera a que la Sub-CPU esté lista para recibir instrucciones
//  Y pone en el puerto comm principal la instruccione
//
//////////////////////////////////////////////////////////////////////////
void wait_do_cmd(char cmd){

    while (read_byte(0xA1200F)) ;   // espera a que la Sub-CPU esté lista para recibir instrucciones
    write_byte(0xA1200E, cmd);      // pone en el puerto comm principal la instruccione
}
