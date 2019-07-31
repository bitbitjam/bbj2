#include "..\main.h"

// retraso del press start
#define delay_press_start   70

// starfield
#define MAX_TYPES 3
#define MAX_STARS 64
#define TILE_USERINDEX  title_logo_tiles_size+1

u8 tblSpeed[MAX_TYPES] = {1,2,3};
u8 tblIndex[MAX_TYPES] = {32,34,36};

typedef struct
{
    u8  speed;
    u8  type;
    SpriteDef   sprite;

} _tStar;


_tStar  tStar[MAX_STARS];
SpriteDef  sStar[MAX_STARS];

u16 scroll;

////////////////////////////////////////////////////////
//
// void exitIntro(){
// salimos de la intro
//
////////////////////////////////////////////////////////
void exitIntro(){

    //VDP_fadeOutAll (60, 0);
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);
    delay(60);
}


////////////////////////////////////////////////////////
//
// void waitBetweenMessages(u8 time)
// pausa entre mensajes
//
////////////////////////////////////////////////////////
u8 waitBetweenMessages(u8 time){

    u8 i;
    unsigned int keys_pad1 = 0;

    for(i = 0; i < time; i++){

        // lee el pad
        keys_pad1 = get_pad(0) & SEGA_CTRL_BUTTONS;

        if( keys_pad1 & SEGA_CTRL_START )
            return 1;

        VDP_waitVSync();
    }

    VDP_clearPlan(APLAN);

    return 0;
}


////////////////////////////////////////////////////////
//
// u8 waitBetweenLetters(u8 time)
// pausa entre letras
//
////////////////////////////////////////////////////////
u8 waitBetweenLetters(u8 time){

    u8 i;
    unsigned int keys_pad1 = 0;

    for( i = 0; i < time; i++ ){

        // lee el pad
        keys_pad1 = get_pad(0) & SEGA_CTRL_BUTTONS;

        if( keys_pad1 & SEGA_CTRL_START )
            return 1;

        VDP_waitVSync();
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//
//  u8 introScreen(){
//  la intro del juego
//
//////////////////////////////////////////////////////////////////////////////////////
u8 introScreen(){

    // cadenas de texto de l intro
    const char *str[num_strings_intro] = {
        "THE DAY IS 26 JULY OF 2015^",

        "POCKETLUCHO IS SPENDING",
        "HIS HOLIDAYS AND NOT CODING",
        "AT ALL^^^ LAZY BASTARD^",

        "SUDDENLY^^^ SOMETHING HAPPENS",
        "AND HE IS ABSORBED BY",
        "HIS OLD 8 BIT COMPUTER^^^",

        "TWO KINDGDOMS ARE BATTLING_",
        "USERS AND PROGRAMS_",
        "FOR THE SURVIVAL OF THEIR FONT^",

        "THE CONFONTED KINDOMGS GAMES",
        "HAS BEGUN^^^ ARIAL OR COMIC SANS",
        "THE FATE OF HUMANITY IS UNCLEAR^^^"
    };

    // numero de cadenas por pantalla
    const u8 num_strings[num_screens_intro] = {1, 3, 3, 3, 3};

    // posicion en pantalla de cada cadena
    const u8 pos_strings[num_strings_intro][2] = {
        {pos_x_strg1, pos_y_strg1},
        {pos_x_strg2, pos_y_strg2}, {pos_x_strg3, pos_y_strg3}, {pos_x_strg4, pos_y_strg4},
        {pos_x_strg5, pos_y_strg5}, {pos_x_strg6, pos_y_strg6}, {pos_x_strg7, pos_y_strg7},
        {pos_x_strg8, pos_y_strg8}, {pos_x_strg9, pos_y_strg9}, {pos_x_strg10, pos_y_strg10},
        {pos_x_strg11, pos_y_strg11},{pos_x_strg12, pos_y_strg12},{pos_x_strg13, pos_y_strg13}
    };

    // graficos de cada pantalla
    const INTRO _introGfxList [] = {
        /*{ 0, 0, 0, 0 },
        { (u16*) map_intro_1, (u32*)tiles_intro_1, (u16*)pal_intro_1, tiles_intro_1_size },
        { (u16*) map_intro_2, (u32*)tiles_intro_2, (u16*)pal_intro_2, tiles_intro_2_size },
        { 0, 0, (u16*)pal_intro_2, 0 },
        { (u16*) map_intro_3, (u32*)tiles_intro_3, (u16*)pal_intro_3, tiles_intro_3_size },
        { (u16*) map_intro_4, (u32*)tiles_intro_4, (u16*)pal_intro_4, tiles_intro_4_size },
        { 0, 0, (u16*)pal_intro_4, 0 },
        { (u16*) map_intro_5, (u32*)tiles_intro_5, (u16*)pal_intro_5, tiles_intro_5_size },
        { 0, 0, 0, tiles_intro_2_size } // no quitar!*/
    };

    // cadena temporal que dibujaremos
    char str_temp[32];

    // para el dibujado de las letras una a una
    u8 a, b, c, num_string;
    u32 len;

    // retraso para hacer casar la musica en 50-60 hz
    u8 delay = 3;

    VDP_resetSprites();
    VDP_updateSprites();

    //clear_screen();
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    VDP_setReg(7, 0x0E);    // cambia el color del fondo

    ////////////////////////////////////////////////////////////////////////
    // cargo la paleta inicial y los tiles de la fuente
    VDP_loadTileData( title_font_tiles, pos_vram_font, font_credits_tiles_size, 1);
    VDP_setPalette((u16 *)title_font_pal, 0, 16);

    ////////////////////////////////////////////////////////////////////////
    // para cada pantalla de la intro
    num_string = 0;
    for( a = 0; a < num_screens_intro; a++ ){

        // dibujamos su ilustracion
        /*if( _introGfxList[a].size != 0 ){

            VDP_initFading1(16, 31, (u16*)_introGfxList[a-1].pal, palette_black, 30);    // from col, to col, pal src, pal dst, numframes);
            while(VDP_doStepFading1())  VDP_waitVSync();

            VDP_loadTileData( (u32*)_introGfxList[a].data, 1, _introGfxList[a].size, 1);
            //VDP_fillTileMap( BPLAN, (u16*)_introGfxList[a].map,  0, 0, tam_map_h_tiles, tam_map_v_tiles+1, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1));  // plano, mapa, x, y, ancho, alto, atributos de tile

            VDP_initFading1(16, 31, palette_black, (u16*)_introGfxList[a-1].pal, 30);    // from col, to col, pal src, pal dst, numframes);
            while(VDP_doStepFading1())  VDP_waitVSync();
            VDP_setPalette((u16 *)_introGfxList[a].pal, 16, 16);
        }*/

        // para cada frase de la pantalla
        for( b = 0; b < num_strings[a]; b++ ){

            // copiamos la cadena que toca
            len = strlen_(str[num_string]);
            strncpy( str_temp, str[num_string], len);

            // dibuja el texto letra a letra
            for(c = 0; c < len; c++){

                // dibuja la letra que toque
                VDP_setTileMap(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font+(int)str_temp[c])-32, c+pos_strings[num_string][0], pos_strings[num_string][1]);

                // espera entre letras y si se pulsa START salimos
                if( waitBetweenLetters(delay) ){
                    exitIntro();
                    return 1;
                }
            }

            num_string++;
        }

        if( _introGfxList[a+1].size == 0 ){
            if( waitBetweenMessages(30) ){
                exitIntro();
                return 1;
            }
        }
        else{
            if( waitBetweenMessages(delayBetweenMessages) ){
                exitIntro();
                return 1;
            }
        }
    }

    exitIntro();

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////
//
// void waitTextBios( u8 time, u16 x, u16 y, u16 pal, u8 sound )
// pausa entre letras y dibujo del cursor
//
///////////////////////////////////////////////////////////////////////////////////////
void waitTextBios( u8 time, u16 x, u16 y, u16 pal, u8 sound ){

    u8 i;
    u8 j = 0;

    for(i = 0; i < time; i++){

        j++;

        if( j == 1 && sound == 1 )
            psgFxPlay(2);

        if( j < 6 ){
            VDP_drawText("=", APLAN, TILE_ATTR_FULL(pal, 1, 0, 0, pos_vram_font), x+1, y);
        }else{
            if( j == 10)
                j = 0;

            VDP_drawText(" ", APLAN, TILE_ATTR_FULL(pal, 1, 0, 0, pos_vram_font), x+1, y);
        }

        psgFxFrame();
        VDP_waitVSync();
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
// void startupScreen(){
// bios falsa del sistema
//
///////////////////////////////////////////////////////////////////////////////////////
void startupScreen(){

    unsigned int keys_pad1 = 0;

    const char *str1 = "MK;1601 mega drive SYSTEM";
    const char *str2 = "COPYRIGHT se3a enterprises 1988";
	const char *str3 = "SYSTEM CONDITION ALL green";
	const char *str4 = "<RUN TRONOW.BIN";

	unsigned char *bios;

    u32 len;
    u16 i;

    cd_unit = 1;

    // pongo todas las paletas a negro
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    // limpio la pantalla
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cargo los tiles comprimidos de la fuente en la vram y asigno la paleta
    VDP_loadTileData( bios_font_tiles, pos_vram_font, font_credits_tiles_size, 1);

    // compruebo si hay unidad de cd
    // Cuando se inserta un cartucho en la MD, el hardware del CD
    // se mapea 0x400000 en vez de 0x000000. Así que la BIOS ROM estará en 0x400000,
    // el banco de la Program RAM estará en 0x420000, y la Word RAM en 0x600000
    bios = (unsigned char *)0x415800;
    if( memcmp(bios + 0x6D, "SEGA", 4 )){

        bios = (unsigned char *)0x416000;
        if( memcmp(bios + 0x6D, "SEGA", 4 )){

            // comprueba si es un WonderMega/X'Eye
            if( memcmp(bios + 0x6D, "WONDER", 6 )){

                bios = (unsigned char *)0x41AD00; // podria estar en 0x40D500
                // comprueba si es un LaserActive
                if( memcmp(bios + 0x6D, "SEGA", 4 )){

                    cd_unit = 0;
                }
            }
        }
    }

    // cargo las paletas
    VDP_setPalette((u16 *)palette_white, 0, 16);
    VDP_setPalette((u16 *)palette_blue, 16, 16);
    VDP_setPalette((u16 *)palette_red, 32, 16);
    VDP_setPalette((u16 *)palette_green, 48, 16);

    // primer mensaje
    len = strlen_(str1);
	for( i = 0; i < len; i++ ){
	    // u16 plan, u16 tile, u16 x, u16 y, u16 w, u16 h
        VDP_fillTileRect( APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font+(int)str1[i]-32), i+2, 2, 1, 1);

        // u8 tiempo, u16 x, u16 y, u16 paleta, u8 suena
        waitTextBios( 3, i+2, 2, PAL0, 1 );
    }

    waitTextBios( 60, i+1, 2, PAL0,1 );

    // segundo mensaje
    len = strlen_(str2);
	for(i = 0; i < len; i++){
        VDP_fillTileRect(APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font+(int)str2[i]-32), i+2, 3, 1, 1);
        waitTextBios( 3, i+2, 3, PAL1, 1 );
    }
    waitTextBios( 60, i+1, 3, PAL1,1 );

    // tercer mensaje
    // const char *str, u16 plan, u16 basetile, u8 pal, u16 x, u16 y
    VDP_drawText("CHECKING SYSTEM...", APLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 2, 5);

    waitTextBios( 60, 19, 5, PAL2, 1 );

	VDP_drawText("64K WRAM 68000:", APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font), 2, 7);
	waitTextBios( 30, 16, 7, PAL0, 1 );

	VDP_drawText("ok", APLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font), 17, 7);
	waitTextBios( 60, 18, 7, PAL3, 1 );

	VDP_drawText("64K VRAM VDP  :", APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font), 2, 8);
	waitTextBios( 30, 16, 8, PAL0, 1 );

	VDP_drawText("ok", APLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font), 17, 8);
	waitTextBios( 40, 18, 8, PAL3, 1 );

	VDP_drawText("8K  WRAM Z80  :", APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font), 2, 9);
	waitTextBios( 30, 16, 9, PAL0, 1 );

	VDP_drawText("ok", APLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font), 17, 9);
	waitTextBios( 40, 18, 9, PAL3, 1 );

	VDP_drawText("CD DRIVE UNIT :", APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font), 2, 10);
	waitTextBios( 30, 16, 10, PAL0, 1 );

    if( cd_unit ){
        VDP_drawText("ok", APLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font), 17, 10);
        waitTextBios( 40, 18, 10, PAL3, 1 );
        waitTextBios( 40, 18, 10, PAL3, 1 );
	}else{
        VDP_drawText("no", APLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 17, 10);
        waitTextBios( 40, 18, 10, PAL2, 1 );
        waitTextBios( 40, 18, 10, PAL2, 1 );
    }

	 // penultimo mensaje
    len = strlen_(str3);
	for(i = 0; i < len; i++){
        VDP_fillTileRect(APLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font+(int)str3[i]-32), i+2, 12, 1, 1);
        waitTextBios( 3, i+2, 12, PAL3, 1 );
    }

	waitTextBios( 40, i+1, 12, PAL3, 1 );

	 // ultimo mensaje
    len = strlen_(str4);
	for( i = 0; i < len; i++ ){
        VDP_fillTileRect(APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font+(int)str4[i]-32), i+2, 14, 1, 1);
        waitTextBios( 6, i+2, 14, PAL1, 1 );

        // desactiva el audio cd
        if(( keys_pad1 & SEGA_CTRL_A ) && ( keys_pad1 & SEGA_CTRL_C ))
            cd_unit = 0;
    }

    waitTextBios( 120, 1, 15, PAL0, 1 );

    if( cd_unit ){
        // Resetea la Sub-CPU, pide el bus
        write_byte(0xA12001, 0x02);
        while (!(read_byte(0xA12001) & 2)) write_byte(0xA12001, 0x02); // espera a la confirmacion del bus


        // Descomprime la BIOS de la Sub-CPU en la Program RAM en 0x00000
        write_word(0xA12002, 0x0002); // no write-protection, bank 0, 2M mode, Word RAM asiganada a la Sub-CPU
        Kos_Decomp(bios, (unsigned char *)0x420000);


        // Copia el programa de la Sub-CPU a la Program RAM en 0x06000
        memcpy((void *)0x426000, &Sub_Start, (int)&Sub_End - (int)&Sub_Start);
        if (memcmp((void *)0x426000, &Sub_Start, (int)&Sub_End - (int)&Sub_Start)){

            VDP_drawText("Fallo al escribir la Program RAM!", APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font), 16, 1);

            while (1) ;
        }

        write_byte(0xA1200E, 0x00); // limpia el puerto comm principal
        write_byte(0xA12002, 0x2A); // proteccion de escritura hasta 0x05400
        write_byte(0xA12001, 0x01); // limpia la peticion del bus, inactiva el reset - permite funcionar al CD Sub-CPU
        while (!(read_byte(0xA12001) & 1)) write_byte(0xA12001, 0x01); // espera a que la Sub-CPU funcione

        // configura la rutina vertical blank para manejar interrupciones de nivel 2 a la Sub-CPU
        // La BIOS de la Sub-CPU las necesita para funcionar
        write_long((unsigned int)&vblank_vector, (unsigned int)&gen_lvl2);
        set_sr(0x2000); // activa interrupciones


        // espera a que el programa de la Sub-CPU ponga en el puerto comm secundario indicando que esta funcionando
        while (read_byte(0xA1200F) != 'I'){

            static int timeout = 0;
            timeout++;
            if (timeout > 20000){
                VDP_drawText("Fallo al iniciar el CD!", APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, pos_vram_font), 16, 1);
                while (1) ;
            }
        }

        // espera a que la Sub-CPU indique que está lista para recibir instrucciones
        while (read_byte(0xA1200F) != 0x00) ;

        // comprueba el disco, si no se hace el hard del cd no detecta las pistas correctamente
        wait_do_cmd('C');
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00);
    }

    // pongo todas las paletas a negro
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);
}


///////////////////////////////////////////////////////////////////////////////////////
//
// void creditsScreen(){
// creditos iniciales del juego
//
///////////////////////////////////////////////////////////////////////////////////////
void creditsScreen(){

    // reseteo las capas, los sprites
    VDP_resetSprites();
    VDP_updateSprites();

    //clear_screen();
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cambia el color del fondo
    VDP_setReg(7, 0x00);

    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    VDP_loadTileData( credits_font_tiles, pos_vram_font, font_credits_tiles_size, 1);


    VDP_drawText("TRO-NOW", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),17,1);
    VDP_drawText("bitbitjam 2015", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),13,4);

    VDP_drawText("code by @POCKET LUCHO", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),10,7);

    VDP_drawText("graphics by @POCKET LUCHO", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),8,9);

    VDP_drawText("sound by @DAVIDBONUS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),10,11);

    //VDP_drawText("logo by @VAKAPP", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0,  pos_vram_font),8,13);

    //VDP_drawText("produced by @1985ALTERNATIVO", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0,  pos_vram_font),2,16);


    VDP_drawText("special thanks to @EMIGRANTEBRUCE,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),3,14);
    VDP_drawText("@SPRITESMIND, @MEGADRIVEDEV,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),6,16);
    VDP_drawText("CHILLYWILLY, SHIRU, @DORAGASU,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),5,18);
    VDP_drawText("@MARC ROBLEDO, @JORDIMONTORNES", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),5,20);
    VDP_drawText("         AND YOU...", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font),7,22);

    VDP_drawText("not produced by license", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 9, 25);
    VDP_drawText("from sega enterprises ltd", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 8, 26);


    // fade in de paleta
    VDP_initFading1(16, 31, palette_black, credits_font_pal, 30);    // from col, to col, pal src, pal dst, ... , numframes);
    while(VDP_doStepFading1()){
        psgFxFrame();
        VDP_waitVSync();
    }
    VDP_setPalette((u16 *)credits_font_pal, 16, 16);

    delay(60*5);

    VDP_initFading1(16, 31, credits_font_pal, palette_black, 30);    // from col, to col, pal src, pal dst, ... , numframes);
    while(VDP_doStepFading1())
        VDP_waitVSync();
}


///////////////////////////////////////////////////////////////////////////////////////
//
// void draw1985Logo()
// dibuja el logo de 1985
//
///////////////////////////////////////////////////////////////////////////////////////
void draw1985Logo(){

   // reseteo las capas, los sprites
    VDP_resetSprites();
    VDP_updateSprites();

    //clear_screen();
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    // cambia el color del fondo
    VDP_setReg(7, 0x0F);

    // cargo los tiles del logo
    VDP_loadTileData(logo1985_tiles, 1, logo1985_tiles_size, 1);

    // dibuja el mapa
    VDP_fillTileMap( APLAN, logo1985_map, 12, 8, 16, 10, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1));  // plano, mapa, x, y, ancho, alto, atributos de tile

    // carga la cancion y la ejecuta
    if( !cd_unit )
        SND_loadSong_XGM(0);
    else{
        // reproduce la pista 21 una vez
        write_word(0xA12010, 21);
        write_byte(0xA12012, 0x00); // reproduce la pista una vez
        wait_do_cmd('P');           // instruccion PlayTrack
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00); // confirmacion recibida del resultado de la instruccion
    }

    delay(90);

    // fade in de paleta
    VDP_initFading1(0, 15, palette_black, logo1985_pal, 30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading1()) VDP_waitVSync();
    VDP_setPalette((u16 *)logo1985_pal, 0, 16);

    delay(90);

    VDP_initFading1(0, 15, logo1985_pal, logo1985_blue_pal, 30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading1()) VDP_waitVSync();
    VDP_setPalette((u16 *)logo1985_blue_pal, 0, 16);

    delay(180);

    // fade out
    VDP_initFading1(0, 15, logo1985_blue_pal, palette_black, 30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading1()) VDP_waitVSync();

    delay(60);
}


///////////////////////////////////////////////////////////////////////////////////////
//
// void drawTextRecords(const char *str, u16 basetile, u8 flag, u16 x, u16 y){
// dibuja el texto de la pantalla de records
//
///////////////////////////////////////////////////////////////////////////////////////
void drawTextRecords(const char *str, u16 basetile, u8 flag, u16 x, u16 y){

    u32 len;
    u16 data[40];
    u16 i;

    len = strlen_(str);

    for (i = 0; i < len; i++) data[i] = (str[i] - 6 + 64*flag);

    VDP_fillTileMap(APLAN, data,  x, y, len, 1, TILE_ATTR_FULL(PAL1, 0, 0, 0, 0));
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  u8 recordsScreen( u8 mode, u32 score ){
//  pantalla de records
//
///////////////////////////////////////////////////////////////////////////////////////
u8 recordsScreen( u8 mode ){

    u8 a, b, c, d, e, f, temp_x;
    unsigned int keys = 0;
    u8 letter = 0, busy = 1, pos = 0, pos_letter = 0;

    const char* alphabet[37] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9","." };

    // para la pantalla de records
    char *positionTable[4];
    char *numbersTable[10];
    char *rankTable[3];

	positionTable[0]="ST";
    positionTable[1]="ND";
    positionTable[2]="RD";
    positionTable[3]="TH";

    numbersTable[0]="1";
    numbersTable[1]="2";
    numbersTable[2]="3";
    numbersTable[3]="4";
    numbersTable[4]="5";
    numbersTable[5]="6";
    numbersTable[6]="7";
    numbersTable[7]="8";
    numbersTable[8]="9";
    numbersTable[9]="10";

    rankTable[0]="EASY";
    rankTable[1]="NORM";
    rankTable[2]="HARD";

   // reseteo las capas, los sprites
    VDP_resetSprites();
    VDP_updateSprites();

    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    ////////////////////////////////////////////////////////////////////////
    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    // cambia el color del fondo
    VDP_setReg(7, 0x0F);

    // restauramos el scroll
    VDP_setVerticalScroll(APLAN, 0, 0);
    VDP_setHorizontalScroll(APLAN, 0, 0);

    // fuente de paleta rotatoria
    VDP_loadTileData( records_text_tiles, pos_vram_tiles_text+1, records_text_tiles_size, 1);
    VDP_setPalette((u16 *)records_text_pal, 16, 16);

    ///////////////////////////////////
    // ciclo la paleta de los records
    u8 segs, ticks;
    u16 temp_color, temp_color2; // color temporal

    // dibujamos poco a poco y ciclamos la paleta
    a = b = c = d = e = f = segs = ticks = 0;

    // modo atract
    if( mode == 0 ){

        // rellena un cuadrado con el logo de 23x1 del tile con paleta 0 en (8,2)
        VDP_loadTileData( records_title_tiles, 1, records_title_tiles_size, 1);
        VDP_fillTileRectInc(APLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, 1), 8, 2, records_title_tiles_size, 1);

        VDP_setPalette((u16 *)records_title_pal, 32, 16);

        // fuente de paleta normal
        VDP_loadTileData( credits_font_tiles, pos_vram_font, font_credits_tiles_size, 1);
        VDP_drawText("RANK      NAME      POINTS    MODE", APLAN, TILE_ATTR_FULL(PAL3, 0, 0, 0, pos_vram_font), 2, 5);
        VDP_setPalette((u16 *)credits_font_pal, 48, 16);

        // para las 3 tablas
        for( f = 0; f < 3; f++ ){

            // borramos las entradas
            VDP_fillTileRectInc(APLAN, 0, 0, 7, 36, 20);

            while(segs < 6){

                // control de salida
                ticks++;
                if( ticks == 60 ){
                    ticks = 0;
                    segs++;
                }

                // ejecucion con retardo
                c++;
                if( c == 8 ){
                    c = 0;

                    /////////////////////////////////////////////////
                    // ciclamos la PAL1 (16-31)
                    temp_color = VDP_getPaletteColor(26);
                    for( d = 26; d > 18; d-- ){
                        temp_color2 = VDP_getPaletteColor(d-1);
                        VDP_setPalette(&temp_color2, d, 1);
                    }

                    VDP_setPalette(&temp_color, 18, 1);

                    // cambiamos la PAL2 (32-47)
                    if( VDP_getPaletteColor(33) == 0x000E )
                       VDP_setPaletteColor(33, 0x0EEE);
                    else
                        VDP_setPaletteColor(33, 0x000E);


                    /////////////////////////////////////////////////
                    // dibujo el texto de los records poco a poco
                    if( a < 10 ){
                        // segun b, dibujo con uno de los 3 tipos de letra disponibles en la fuente

                        // posicion
                        if( a < 4 )
                            drawTextRecords(positionTable[a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 3, 7+2*a);
                        else
                            drawTextRecords(positionTable[3], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 3, 7+2*a);

                        drawTextRecords(".", TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 5, 7+2*a);
                        if( a != 9 )
                            drawTextRecords(numbersTable[a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 2, 7+2*a);
                        else
                            drawTextRecords(numbersTable[a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 1, 7+2*a);

                        // nombre
                        drawTextRecords(namesTable[f][a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 10, 7+2*a);

                        // puntuacion
                        if( pointsTable[f][a] > 99999 )
                            temp_x = 0;
                        else if( pointsTable[f][a] > 9999 )
                            temp_x = 1;
                        else if( pointsTable[f][a] > 999 )
                            temp_x = 2;
                        else if( pointsTable[f][a] > 99 )
                            temp_x = 3;
                        else if( pointsTable[f][a] > 9 )
                            temp_x = 4;
                        else
                            temp_x = 5;

                        drawTextRecords("000000", TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 22, 7+2*a);
                        drawTextRecords(itoa(pointsTable[f][a]), TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 22+temp_x, 7+2*a);

                        // rank
                        drawTextRecords(rankTable[ranksTable[f][a]], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 32, 7+2*a);

                        b++;
                        b %= 3;
                    }
                    a++;
                }

                //////////////////////////////////////////////////
                // salida por START
                keys = get_pad(0) & SEGA_CTRL_BUTTONS;  // lee el pad

                if(keys & SEGA_CTRL_START && busy == 0 )
                    segs = f = 6;
                else if(!(keys & SEGA_CTRL_START))
                    busy = 0;

                // sincroniza la pantalla
                VDP_waitVSync();
            }

            a = b = c = d = e = segs = ticks = 0;
        }
    }
    else{

        // calculamos la posicion de la nueva puntuacion
        if( score == 0 )
            return 0;

        {while( score <= pointsTable[speed][pos] )
            pos++;

        // si no hay record salimos
        if( pos > 9 )
            return 0;

        // movemos los registros antiguos a su nueva posicion
        b = 9;
        while( b > pos ){
            pointsTable[speed][b] = pointsTable[speed][b-1];
            strcpy(namesTable[speed][b], namesTable[speed][b-1]);
            b--;
        }

        // nuevo registro
        pointsTable[speed][pos] = score;
        strcpy(namesTable[speed][pos], "A........");

        // rellena un cuadrado con el logo de 23x1 del tile con paleta 0 en (8,2)
        VDP_loadTileData( records_title_b_tiles, 1, records_title_tiles_size, 1);
        VDP_fillTileRectInc(APLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, 1), 8, 2, records_title_tiles_size, 1);

        VDP_setPalette((u16 *)records_title_pal, 32, 16);

        // fuente de paleta normal
        VDP_loadTileData( credits_font_tiles, pos_vram_font, font_credits_tiles_size, 1);
        VDP_drawText("RANK      NAME      POINTS    MODE", APLAN, TILE_ATTR_FULL(PAL3, 0, 0, 0, pos_vram_font), 2, 5);
        VDP_setPalette((u16 *)credits_font_pal, 48, 16);

        // dibujo el texto de los records
        a = b = 0;
        while( a < 10 ){
            // segun b, dibujo con uno de los 3 tipos de letra disponibles en la fuente
            // posicion
            if( a < 4 )
                drawTextRecords(positionTable[a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 3, 7+2*a);
            else
                drawTextRecords(positionTable[3], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 3, 7+2*a);

            drawTextRecords(".", TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 5, 7+2*a);
            if( a != 9 )
                drawTextRecords(numbersTable[a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 2, 7+2*a);
            else
                drawTextRecords(numbersTable[a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 1, 7+2*a);

            // nombre
            drawTextRecords(namesTable[speed][a], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 10, 7+2*a);

            // puntuacion
            if( pointsTable[speed][a] > 99999 )
                temp_x = 0;
            else if( pointsTable[speed][a] > 9999 )
                temp_x = 1;
            else if( pointsTable[speed][a] > 999 )
                temp_x = 2;
            else if( pointsTable[speed][a] > 99 )
                temp_x = 3;
            else if( pointsTable[speed][a] > 9 )
                temp_x = 4;
            else
                temp_x = 5;

            drawTextRecords("000000", TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 22, 7+2*a);
            drawTextRecords(itoa(pointsTable[speed][a]), TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 22+temp_x, 7+2*a);

            // rank
            drawTextRecords(rankTable[ranksTable[speed][a]], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), b, 32, 7+2*a);

            b++;
            b %= 3;

            a++;
        }}

        // dibujamos las entradas
        a = b = 0;
        while(1){
            c++;

            // parpadeo de la letra actual
            if( c == 3)
                drawTextRecords(" ", TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 10+pos_letter, 7+2*pos);
            else if( c == 8)
                drawTextRecords(alphabet[letter], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 10+pos_letter, 7+2*pos);
                //drawTextRecords(namesTable[speed][pos], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 8, 7+2*pos);

            if( c == 8 ){
                c = 0;

                /////////////////////////////////////////////////
                // ciclamos la PAL1 (16-31)
                temp_color = VDP_getPaletteColor(26);
                for( d = 26; d > 18; d-- ){
                    temp_color2 = VDP_getPaletteColor(d-1);
                    VDP_setPalette(&temp_color2, d, 1);
                }

                VDP_setPalette(&temp_color, 18, 1);

                // cambiamos la PAL2 (32-47)
                if( VDP_getPaletteColor(33) == 0x000E )
                    VDP_setPaletteColor(33, 0x0EEE);
                else
                    VDP_setPaletteColor(33, 0x000E);
            }

            //////////////////////////////////////////////////
            // cambio de letra
            keys = get_pad(0) & SEGA_CTRL_BUTTONS;  // lee el pad

            // eleccion de letra
            if(keys & SEGA_CTRL_RIGHT && busy == 0 ){
                if( letter != 35 )
                    letter++;
                else
                    letter = 0;
                busy = 1;

                // las mayusculas van de 65 a 90, los numeros de 48 a 57
                /*if( letter < 26)
                    namesTable[speed][pos][pos_letter] = letter+65;
                else
                    namesTable[speed][pos][pos_letter] = letter+22;*/

                //drawTextRecords(namesTable[speed][pos], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 8, 7+2*pos);
            }
            else if(keys & SEGA_CTRL_LEFT && busy == 0 ){
                if( letter > 0 )
                    letter--;
                else
                    letter = 35;
                busy = 1;

                // las mayusculas van de 65 a 90, los numeros de 48 a 57
                /*if( letter < 26)
                    namesTable[speed][pos][pos_letter] = letter+65;
                else
                    namesTable[speed][pos][pos_letter] = letter+22;*/

                //drawTextRecords(namesTable[speed][pos], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 8, 7+2*pos);
            }
            // pasar a siguiente posicion
            else if(keys & SEGA_CTRL_A && busy == 0 ){

                busy = 1;

                if( letter < 26)
                    namesTable[speed][pos][pos_letter] = letter+65;
                else
                    namesTable[speed][pos][pos_letter] = letter+22;

                if( pos_letter < 8 ){
                    pos_letter++;

                    letter = 0;

                    //namesTable[speed][pos][pos_letter] = letter+65;

                    drawTextRecords(namesTable[speed][pos], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 10, 7+2*pos);
                }
            }
            // borrar
            else if((keys & SEGA_CTRL_B && busy == 0 ) || (keys & SEGA_CTRL_UP && busy == 0 ) ){

                busy = 1;

                if( pos_letter > 0 ){

                    namesTable[speed][pos][pos_letter] = 46; // punto
                    drawTextRecords(".", TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 10+pos_letter, 7+2*pos);

                    pos_letter--;

                    letter =  namesTable[speed][pos][pos_letter]-65;
                    if( letter > 90 ) letter =  namesTable[speed][pos][pos_letter]-22;

                    //drawTextRecords(namesTable[speed][pos], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 8, 7+2*pos);
                }
            }
            // salir
            else if( (keys & SEGA_CTRL_START && busy == 0 && pos_letter > 0)){
                SRAM_enable();
                writeRecordsTableSRAM();
                SRAM_disable();
                break;
            }

            // para evitar repeticiones
            if( !(keys & SEGA_CTRL_LEFT) && !(keys & SEGA_CTRL_RIGHT) && !(keys & SEGA_CTRL_B) && !(keys & SEGA_CTRL_A) && !(keys & SEGA_CTRL_UP) && !(keys & SEGA_CTRL_START) )
                busy = 0;

            //drawTextRecords(alphabet[letter], TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_tiles_text), pos % 3, 8+pos_letter, 7+2*pos);
            // salida por tiempo
            if( pos_letter == 0 ){

                ticks++;
                if( ticks == 60 ){
                    ticks = 0;
                    segs++;
                }

                if( segs == 30 ){
                    strcpy(namesTable[speed][pos], "PLAYER...");
                    mode = 0;   // no vamos al QR
                    break;
                }
            }


            // sincroniza la pantalla
            VDP_waitVSync();
        }
    }

    // fade out
    VDP_initFading3(16, 31, records_text_pal, palette_black, 32, 47, records_title_pal, palette_black, 48, 63, credits_font_pal, palette_black, 30);
    do VDP_waitVSync(); while(VDP_doStepFading3());

    // generamos el QR
    //if( mode )
    //    generateQR(pos);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  void resetStar( u8 i )
//  resetea la estrella al salir de la pantalla
//
///////////////////////////////////////////////////////////////////////////////////////
void resetStar( u8 i ){

    tStar[i].type = random () % MAX_TYPES;
    tStar[i].speed = tblSpeed[tStar[i].type];
    tStar[i].sprite.posx = screen_width;
    tStar[i].sprite.posy = random () % screen_height;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  void updateStars()
//  actualiza las estrellas
//
///////////////////////////////////////////////////////////////////////////////////////
void updateStars(){
    u8 i;

    for( i = 0; i < MAX_STARS; i++ ){

        tStar[i].sprite.posx -= tStar[i].speed;

        if( tStar[i].sprite.posx <= 0 )
            resetStar( i );

        VDP_setSpritePosition( i, tStar[i].sprite.posx, tStar[i].sprite.posy );
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  void initStarfield(){
//  inicia los sprites del starfield
//
///////////////////////////////////////////////////////////////////////////////////////
void initStarfield(){
    u8 i;

    for(i = 0; i < MAX_STARS; i++ ){

        tStar[i].type = random () % MAX_TYPES;
        tStar[i].speed = tblSpeed[ tStar[i].type ];

        tStar[i].sprite.posx = random () % screen_width;
        tStar[i].sprite.posy = random () % screen_height;
        tStar[i].sprite.size = SPRITE_SIZE(1, 1);
        tStar[i].sprite.tile_attr = TILE_ATTR_FULL( PAL1, 0, 0, 0, TILE_USERINDEX + tStar[i].type );
        tStar[i].sprite.link  = i+1;
        VDP_setSpriteP(i, &tStar[i].sprite);
    }
}


//////////////////////////////////////////////////////////////////////////////////////
//
//  u8 titleScreen(){
//  pantalla de titulo
//
//////////////////////////////////////////////////////////////////////////////////////
u8 titleScreen(){

    u8 counter = 0, pstart = 1, ticks = 0, segs = 0;
    unsigned int keys = 0;

    // para el efecto persiana
    u8 bg_assemble_start_line;

    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    // borro los planos
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cambia el color del fondo
    VDP_setReg(7, 0x0F);

    // cambio el registro 11 del vdp para el raster
    vscroll_a = 0;
    VDP_setReg(11, 0x00);

    // cargo los tiles del logo
    VDP_loadTileData(title_logo_tiles, 1, title_logo_tiles_size, 1);

    // dibuja el mapa
    VDP_fillTileMap( APLAN, title_logo_map, 4, 4, title_logo_tile_x_size, title_logo_tile_y_size, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1));  // plano, mapa, x, y, ancho, alto, atributos de tile

    // fuente
    VDP_loadTileData( title_font_tiles, pos_vram_font, font_credits_tiles_size, 1);

    VDP_drawText("1985ALTERNATIVO", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0,  pos_vram_font), 12, 26);

    VDP_drawText("BITBITJAM COMPO 2015", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0,  pos_vram_font), 10, 9);

    // estrellas
    VDP_loadTileData(stars_tiles, TILE_USERINDEX, 3, 1);

    // linea en la que empieza el efecto
    bg_assemble_start_line = 72;

    // seleccionamos la rutina a ejecutar en la interrupcion HSYNC
    HBlankCallbackSet(BgAsmHIntCB);
    VDP_setHIntCounter(0);                                  // interrupcion cada scanline
    VDP_setHInterrupt(1);                                   // activa la interrupcion HINT

    /////////////////////////////////////////
    // bucle de dibujado del logo
    while(bg_assemble_start_line > 1){

        // esperamos a VSync para actualizar lo relacionado con el dibujado del efecto
        VDP_waitVSync();

        if( bg_assemble_start_line == 71 )
            VDP_setPalette((u16 *)title_logo_pal, 0, 16);

        // cada frame mueve la "cortina" una linea hacia arriba.
        // cuando termina, desactiva la interrupcion HINT
        start_line = --bg_assemble_start_line;
        VDP_setVerticalScroll(APLAN, 0, vscroll_a + start_line--);
    }
    VDP_setHInterrupt(0);

    // inicia el starfield
    initStarfield();

    // fade in de paleta
    VDP_initFading1(16, 31, palette_black, title_logo_pal, 30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading1()){
        updateStars();
        VDP_waitVSync();
    }
    VDP_setPalette((u16 *)title_logo_pal, 0, 16);

    while(1){

        // salida por tiempo
	    ticks++;
	    if( ticks == 60 ){

            ticks = 0;
            segs++;
            if( segs == delay_press_start )
                break;
	    }

		// lee el pad
		keys = get_pad(0) & SEGA_CTRL_BUTTONS;

		// si se pulsa Start o A salimos
		if(keys & SEGA_CTRL_START )
			return 1;

       // parpadeo del press start
		counter++;
		if( counter == 40 ){

			if ( pstart )
                VDP_drawText("INSERT COIN", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0,  pos_vram_font), 14, 17);
            else
                VDP_drawText("           ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0,  pos_vram_font), 14, 17);

			pstart = !pstart;
			counter = 0;
		}

		updateStars();

        VDP_waitVSync();
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  void drawExplanationTextMenu(u8 mode, u8 letter){
//  dibujo los texto de explicacion de la pantalla de menu
//
///////////////////////////////////////////////////////////////////////////////////////
void drawExplanationTextMenu( u8 mode, u8 letter){

    // cadenas de texto
    const char *str[2] = {
        "SELECT NUMBER OF RIDERS      ON THE LIGHT CYCLE GRID",
        "SELECT NUMBER OF USERS       FIGHTING FOR SURVIVAL  "
    };

    // cadena de texto temporal que dibujaremos
    char str_temp[54];

    // para el dibujado de las letras una a una
    u8 len = 0;

    // copiamos la cadena que usaremos en la temporal
    len = strlen_(str[mode]);
    strncpy( str_temp, str[mode], len);

    // dibuja el texto letra a letra
    if( letter < len ){
        if( letter < 28)
            VDP_setTileMap(BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font+(int)str_temp[letter])-32, letter+8, 23);
        else
            VDP_setTileMap(BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font+(int)str_temp[letter])-32, letter-21, 25);
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  u8 menuScreen( u8 *num_players, u8 *num_human_players ){
//  pantalla de menu
//
///////////////////////////////////////////////////////////////////////////////////////
void menuScreen( u8 *num_players, u8 *num_human_players ){

    unsigned int keys = 0;
    u8 keypress = 1;
    u8 scroll = 0, a, b;

    u8 letter = 0;                  // para el texto explicativo
    u8 ticks = 0;

    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    // para la musica
    if( !cd_unit )
        SND_stopPlay_XGM();
    else{
        wait_do_cmd('S');           // instruccion StopPlaying
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00);
    }

    psgFxPlay(0);

    // borro los planos
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cambia el color del fondo
    VDP_setReg(7, 0x0F);

    // activo el modo HS
    VDP_setHilightShadow(1);

    // para que no se vea oscuro por el shadow dibujo toda la pantalla vacia en alta prioridad
    VDP_fillTileRect(BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, 0), 0, 0, 64, 32);

    // fuente
    VDP_loadTileData( title_font_tiles, pos_vram_font, font_credits_tiles_size, 1);

    // version
    VDP_drawText( version, BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 1, 1);
    VDP_drawText("SELECT SCREEN", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 26, 1);

    VDP_drawText("!\"#", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
    VDP_drawText("$%&", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);

    VDP_drawText( "2 RIDERS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 10);
    VDP_drawText( "4 RIDERS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 14);

    // cargo los tiles del fondo
    VDP_loadTileData(menu_bg_tiles, 1, menu_bg_tiles_size, 1);

    // dibuja el mapa
    VDP_fillTileMap( APLAN, menu_bg_map, 0, 0, menu_bg_tile_x_size, menu_bg_tile_y_size, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1));  // plano, mapa, x, y, ancho, alto, atributos de tile

    // carga los tiles del SPRITE del fondo del menu transparente
    VDP_loadTileData( sprite_menu_tiles, 1+menu_bg_tiles_size, sprite_menu_tiles_size, 1);

    // sprites del menu
    {sprite_menu[0].posx = 96;
    sprite_menu[0].posy = 52;

    sprite_menu[1].posx = 96;
    sprite_menu[1].posy = 84;

    sprite_menu[2].posx = 96;
    sprite_menu[2].posy = 116;

    sprite_menu[3].posx = 128;
    sprite_menu[3].posy = 52;

    sprite_menu[4].posx = 128;
    sprite_menu[4].posy = 84;

    sprite_menu[5].posx = 128;
    sprite_menu[5].posy = 116;

    sprite_menu[6].posx = 160;
    sprite_menu[6].posy = 52;

    sprite_menu[7].posx = 160;
    sprite_menu[7].posy = 84;

    sprite_menu[8].posx = 160;
    sprite_menu[8].posy = 116;

    sprite_menu[9].posx = 192;
    sprite_menu[9].posy = 52;

    sprite_menu[10].posx = 192;
    sprite_menu[10].posy = 84;

    sprite_menu[11].posx = 192;
    sprite_menu[11].posy = 116;

    // 12 a 18 texto explicativo
    a = 0;
    for( b = 12; b < 19; b++){
        sprite_menu[b].posx = 48+32*a;
        sprite_menu[b].posy = 180;
        a++;
    }

    // extra para los logros
    for( b = 19; b < 44; b++){
        sprite_menu[b].posx = screen_width;
        sprite_menu[b].posy = 0;
    }

    // valores comunes
    for( a = 0; a < 43; a++){
        sprite_menu[a].size = SPRITE_SIZE(4,4);
        sprite_menu[a].tile_attr = TILE_ATTR_FULL(PAL3, 0, 0, 0, 1+menu_bg_tiles_size); // paleta, prioridad, vflip, hflip, tile 1
        sprite_menu[a].link  = a+1;

        VDP_setSpriteP(a, &sprite_menu[a]);
    }

    // el ultimo sprite apunta a si mismo
    sprite_menu[42].link = 42; VDP_setSpriteP(42, &sprite_menu[42]);

    VDP_updateSprites();}

    if( !cd_unit )
        SND_loadSong_XGM(3);
    else{
        // reproduce la pista 8
        write_word(0xA12010, 8);
        write_byte(0xA12012, 0x01); // reproduce la pista y repite
        wait_do_cmd('P');           // instruccion PlayTrack
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00); // confirmacion recibida del resultado de la instruccion
    }

    // reiniciamos el numero de jugadores
    *num_players = 2;
    *num_human_players = 1;

    // fade in de paleta
    VDP_initFading1(16, 31, palette_black, title_font_pal, 30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading1()){
        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        psgFxFrame();
        VDP_waitVSync();
    }
    VDP_setPalette((u16 *)title_font_pal, 16, 16);

    // seleccionamos numero de jugadores
    while(1){

        // lee el pad
		keys = get_pad(0) & SEGA_CTRL_BUTTONS;

        ////////////////////////////////////
        // cambiamos de modo
        if(keys & SEGA_CTRL_DOWN && keypress == 0 ){

            VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
            VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);
            VDP_drawText("!\"#", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 13);
            VDP_drawText("$%&", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 14);

            keypress = 1;
            *num_players = 4;

            psgFxPlay(10);
        }
        else if(keys & SEGA_CTRL_UP && keypress == 0 ){

            VDP_drawText("!\"#", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
            VDP_drawText("$%&", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);
            VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 13);
            VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 14);

            keypress = 1;
            *num_players = 2;

            psgFxPlay(10);
        }
        else if(!(keys & SEGA_CTRL_DOWN) && !(keys & SEGA_CTRL_UP) && !(keys & SEGA_CTRL_A))
            keypress = 0;

        ////////////////////////////////////
        // elegimos modo
        if(keys & SEGA_CTRL_A && keypress == 0){

            VDP_drawText( "                             ", BPLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font), 4, 23);
            VDP_drawText( "                             ", BPLAN, TILE_ATTR_FULL(PAL3, 1, 0, 0, pos_vram_font), 4, 25);

            VDP_drawText( "        ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 10);
            VDP_drawText( "        ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 14);

            // 2 jugadores
            if( *num_players == 2 ){

                VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
                VDP_drawText("*+,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 13);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 14);

                VDP_drawText( "1 USER", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 10);
                VDP_drawText( "2 USERS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 14);
            }
            else{

                VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 8);
                VDP_drawText("*+,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 13);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 14);

                VDP_drawText( "1 USER", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 9);
                VDP_drawText( "2 USERS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 11);
                VDP_drawText( "3 USERS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 13);
                VDP_drawText( "4 USERS", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 18, 15);
            }

            psgFxPlay(1);

            keypress = 1;
            letter = 0;

            break;
        }

        ////////////////////////////////////
        // movemos el scroll
        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        ////////////////////////////////////
        // dibujamos el texto explicativo
        if( ticks % 4 == 0 ){
            drawExplanationTextMenu( 0, letter );
            letter++;
        }
        ticks++;

        psgFxFrame();
        VDP_waitVSync();
    }

    // seleccionamos numero de jugadores humanos
    while(1){

        // lee el pad
		keys = get_pad(0) & SEGA_CTRL_BUTTONS;

        if(keys & SEGA_CTRL_DOWN && keypress == 0 ){

            if( *num_players == 2 ){
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);
                VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 13);
                VDP_drawText("*+,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 14);

                *num_human_players = 2;

                psgFxPlay(10);
            }
            else{
                if( *num_human_players < 4 ){
                    VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 6+*num_human_players*2);
                    VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 7+*num_human_players*2);

                    *num_human_players += 1;

                    VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 6+*num_human_players*2);
                    VDP_drawText("*+,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 7+*num_human_players*2);

                    psgFxPlay(10);
                }
            }

            keypress = 1;
        }
        else if(keys & SEGA_CTRL_UP && keypress == 0 ){

            if( *num_players == 2 ){
                VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 9);
                VDP_drawText("*+,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 10);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 13);
                VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 14);
                *num_human_players = 1;

                psgFxPlay(2);
            }
            else{
                if( *num_human_players > 1 ){
                    VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 6+*num_human_players*2);
                    VDP_drawText("   ", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 7+*num_human_players*2);

                    *num_human_players -= 1;

                    VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 6+*num_human_players*2);
                    VDP_drawText("*+,", BPLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, pos_vram_font), 14, 7+*num_human_players*2);

                    psgFxPlay(10);
                }
            }

            keypress = 1;
        }
        else if(!(keys & SEGA_CTRL_DOWN) && !(keys & SEGA_CTRL_UP) && !(keys & SEGA_CTRL_A))
            keypress = 0;

        ////////////////////////////////////
        // elegimos modo
        if(keys & SEGA_CTRL_A && keypress == 0)
            break;

        ////////////////////////////////////
        // movemos el scroll
        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        ////////////////////////////////////
        // dibujamos el texto explicativo
        if( ticks % 4 == 0 ){
            drawExplanationTextMenu( 1, letter );
            letter++;
        }
        ticks++;

        psgFxFrame();
        VDP_waitVSync();
    }

    psgFxPlay(1);

    // fade in de paleta
    VDP_initFading1(16, 31, title_font_pal, palette_black,  30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading1()){
        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        psgFxFrame();
        VDP_waitVSync();
    }

    // activo el modo HS
    VDP_setHilightShadow(0);
}


