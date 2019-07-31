#include "..\main.h"

/////////////////////////////////////////////////////////////////
//
// void initRecords(){
// valores iniciales de la pantalla de records
//
/////////////////////////////////////////////////////////////////
void initRecords(){

    u8 a;

    for( a = 0; a < 3; a++ ){
        strcpy(namesTable[a][0], "MAYOR....");
        strcpy(namesTable[a][1], "SANCHEZ..");
        strcpy(namesTable[a][2], "MEAD.....");
        strcpy(namesTable[a][3], "GIRAUD...");
        strcpy(namesTable[a][4], "LLOYD....");
        strcpy(namesTable[a][5], "FLYNN....");
        strcpy(namesTable[a][6], "LISBERGER");
        strcpy(namesTable[a][7], "KUSHNER..");
        strcpy(namesTable[a][8], "MACBIRD..");
        strcpy(namesTable[a][9], "TAYLOR...");

        pointsTable[a][0] = 300;
        pointsTable[a][1] = 200;
        pointsTable[a][2] = 100;
        pointsTable[a][3] = 75;
        pointsTable[a][4] = 50;
        pointsTable[a][5] = 25;
        pointsTable[a][6] = 20;
        pointsTable[a][7] = 15;
        pointsTable[a][8] = 10;
        pointsTable[a][9] = 5;

        ranksTable[a][0] = a;
        ranksTable[a][1] = a;
        ranksTable[a][2] = a;
        ranksTable[a][3] = a;
        ranksTable[a][4] = a;
        ranksTable[a][5] = a;
        ranksTable[a][6] = a;
        ranksTable[a][7] = a;
        ranksTable[a][8] = a;
        ranksTable[a][9] = a;
    }

    // Inicializar logros a 0
    ad.achievements = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void initGame( u8 num_players )
// doy valor inicial a las variables del juego (personajes...)
// y carga los tiles de los personajes e inicia sus sprites
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initGame( u8 num_players ){

    u8 a, b;

    // reseteo las capas, los sprites
    VDP_resetSprites();
    VDP_updateSprites();

    // para la musica
    if( !cd_unit )
        SND_stopPlay_XGM();
    else{
        wait_do_cmd('S');           // instruccion StopPlaying
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00);
    }

    //clear_screen();
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    // cargo la paleta inicial
    VDP_setPalette((u16 *)palette_black, 0, 16);
    VDP_setPalette((u16 *)palette_black, 16, 16);
    VDP_setPalette((u16 *)palette_black, 32, 16);
    VDP_setPalette((u16 *)palette_black, 48, 16);

    // scroll
    VDP_setVerticalScroll(APLAN, 0, +1);
    VDP_setHorizontalScroll(APLAN, 0, -1);

    /////////////////////////////////////
    //  mapa
    VDP_loadTileData(bg_00_tiles, 1, bg_00_tiles_size, 1);

    // iniciamos la zona de juego a 0
    for( a = 0; a < screen_width_tiles; a++ )
        for( b = 0; b < screen_height_tiles; b++ )
            grid[a][b] = 0;

    // dibuja el mapa
    VDP_fillTileMap( BPLAN, bg_00_map, 0, 1, bg_00_tile_x_size, bg_00_tile_y_size, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1));  // plano, mapa, x, y, ancho, alto, atributos de tile

    /////////////////////////////////////
    // sprites de los jugadores
    VDP_loadTileData(sprites_00_tiles, tile_player_pos, sprites_00_tiles_size, 1);

    // 2 jugadores
    // 0 -   / 2
    //   - 1 / 3
    if( num_players == 2 ){

        player[0].init_posx = 64;
        player[1].init_posx = player[0].init_posx+24*8;
        player[2].init_posx = screen_width+16;
        player[3].init_posx = screen_width+16;

        player[0].init_posy = 24-1;
        player[1].init_posy = player[0].init_posy+24*8+1;
        player[2].init_posy = 0;
        player[3].init_posy = 0;
    }
    // 4 jugadores
    // 0 - 1
    // 2 - 3
    else{
        player[0].init_posx = 64;
        player[1].init_posx = screen_width-64;
        player[2].init_posx = 64;
        player[3].init_posx = screen_width-64;

        player[0].init_posy = 23;
        player[1].init_posy = 23;
        player[2].init_posy = screen_height-7;
        player[3].init_posy = screen_height-7;
    }

    for( a = 0; a < max_num_players; a++ ){
        player[a].posx = FIX32(player[a].init_posx);
        player[a].posy = FIX32(player[a].init_posy);

        player[a].sprite.posx = fix32ToInt(player[a].posx)-4;
        player[a].sprite.posy = fix32ToInt(player[a].posy)-8;
    }

    for( a = 0; a < max_num_players; a++ ){

        player[a].sprite.size = SPRITE_SIZE(1, 2);
        player[a].speed = FIX32(speed_1);
        player[a].joy_press = player[a].frame = player[a].score = 0;
        player[a].state = 1;

        // IA
        player[a].segs = player[a].ticks = 0;
        player[a].segs_obj = random() % 8;

        // direccion inicial depende del numero de jugadores
        if( num_players == 2 ){
              if( a < 1 ){
                player[a].dir = player[a].new_dir = 2;
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_player_pos+a*4);
            }
            else{
                player[a].dir = player[a].new_dir = 0;
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 1, 1, tile_player_pos+a*4);
            }
        }
        else{

            if( a < 2 ){
                player[a].dir = player[a].new_dir = 2;
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_player_pos+a*4);
            }
            else{
                player[a].dir = player[a].new_dir = 0;
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 1, 1, tile_player_pos+a*4);
            }
        }

        player[a].sprite.link  = players_sprite_index+a+1;
        VDP_setSpriteP(players_sprite_index+a, &player[a].sprite);
    };

    /////////////////////////////////////
    // sprites de las explosiones
    VDP_loadTileData(sprites_explosion_tiles, tile_player_explosion_pos, sprites_explosion_tiles_size, 1);

    VDP_updateSprites();

    // fuente
    VDP_loadTileData( title_font_tiles, pos_vram_font, font_credits_tiles_size, 1);

    VDP_drawText("!\"#       '()        -./       =>?", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 0, 0);
    VDP_drawText("$%& 00000 *+, 00000  :;< 00000 [\\] 00000", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 0, 1);

    // inicia el starfield
    VDP_loadTileData( stars_tiles, tile_stars_pos, stars_tiles_size, 1);
    initStarfield3D();

    // mcp
    VDP_loadTileData( mcp_tiles, mcp_tiles_pos, mcp_tiles_size, 1);

    if( !cd_unit )
        SND_loadSong_XGM(2);
    else{
        // reproduce la pista 13
        write_word(0xA12010, 13);
        write_byte(0xA12012, 0x01); // reproduce la pista y repite
        wait_do_cmd('P');           // instruccion PlayTrack
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00); // confirmacion recibida del resultado de la instruccion
    }

    // fade in de paleta
    VDP_initFading3(0, 15, palette_black, bg_00_pal, 16, 31, palette_black, sprites_00_pal, 32, 47, palette_black, title_font_pal,30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading3()){
        updateStars3D(0);
        psgFxFrame();
        VDP_waitVSync();
    }
    VDP_setPalette((u16 *) bg_00_pal, 0, 16);
    VDP_setPalette((u16 *) sprites_00_pal, 16, 16);
    VDP_setPalette((u16 *)title_font_pal, 32, 16);
    VDP_setPalette((u16 *)mcp_pal, 48, 16);
}

