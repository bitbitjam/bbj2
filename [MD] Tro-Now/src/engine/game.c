#include "..\main.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  void initStarfield(){
//  inicia los sprites del starfield
//
///////////////////////////////////////////////////////////////////////////////////////
void initStarfield3D(){

    u8 i;

    for(i = 0; i < MAX_STARS; i++){

        // PRE - CALC
        s16 x_3D = random() % 6400 - 3200;  // Rango: -SCREEN_W *10 <--> SCREEN_W *10
        s16 y_3D = random() % 4480 - 2240;  // Rango: -SCREEN_H *10 <--> SCREEN_H *10 4480 - 2240;
        u8 speed = 1 + random() % 1;
        u8 mov = 0;
        s8 z;

        for (z = MAX_Z; z >= 0; z -= speed){

            s16 x_2D = ORIGIN_X + ((z > 0) ? (x_3D / z) : (x_3D));  // evita "Divide by Zero"
            s16 y_2D = ORIGIN_Y + ((z > 0) ? (y_3D / z) : (y_3D));  // evita "Divide by Zero"

            if( x_2D  >= (0-MAX_SCROLL_X) && x_2D  < (screen_width+MAX_SCROLL_X) && y_2D >= (0-MAX_SCROLL_Y) && y_2D < (screen_height+MAX_SCROLL_Y) ){
                 tStars[i].pos2D[mov].x = x_2D;
                 tStars[i].pos2D[mov].y = y_2D;
                 mov++;
            }
            else
                break;
        }

        // da valor a los sprites
        tStars[i].max_moves = mov;
        tStars[i].idx_pos = random() % tStars[i].max_moves;

        tStars[i].sprite.posx = screen_width;
        tStars[i].sprite.posy = 0;
        tStars[i].sprite.size = SPRITE_SIZE(1, 1);
        tStars[i].sprite.tile_attr = TILE_ATTR_FULL( PAL2, 1, 0, 0, tile_stars_pos + (random() % MAX_TYPES) );
        tStars[i].sprite.link  = i+max_num_players+1;
        VDP_setSpriteP(i+4, &tStars[i].sprite);
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  void updateStars( unsigned int keys ){
//  actualiza las estrellas
//
///////////////////////////////////////////////////////////////////////////////////////
void updateStars3D( u8 num_players ){

    u8 i, a = 0;

    if(scroll_x > 0)  --scroll_x;
    else if(scroll_x < 0)  ++scroll_x;

    if(scroll_y > 0) --scroll_y;
    else if(scroll_y < 0) ++scroll_y;

    // mueve las estrellas
    for(i = 0; i < MAX_STARS; i++){
        u8 *idx_pos = &tStars[i].idx_pos;
        u8 max_moves = tStars[i].max_moves;
        Vect2D_u16* star_pos = &tStars[i].pos2D[*idx_pos];

        //UPDATE POS
        VDP_setSpritePosition(i+max_num_players,  star_pos->x+scroll_x,  star_pos->y+scroll_y);

        if( ++(*idx_pos) >= max_moves) { *idx_pos = 0; }
    }

    // calcula la maxima puntuacion para que las estrellas lo sigan
    if( num_players < 3 ){

        if( player[0].score > player[1].score )
            a = 0;
        else
            a = 1;
    }
    else{
        if( player[0].score > player[1].score && player[0].score > player[2].score && player[0].score > player[3].score )
            a = 0;
        else{
            if( player[1].score > player[0].score && player[1].score > player[2].score && player[1].score > player[3].score )
                a = 1;
            else{
                if( player[2].score > player[0].score && player[2].score > player[1].score && player[2].score > player[3].score )
                    a = 2;
                else
                    a = 3;
            }
        }
    }

    if( player[a].dir == 0 ){
        if( (scroll_y += 2) >=   MAX_SCROLL_Y ) scroll_y -= 1;}
    else if( player[a].dir == 1 ){
        if( (scroll_x -= 2) <= -MAX_SCROLL_X ) scroll_x += 1;}
    else if( player[a].dir == 2 ){
        if( (scroll_y -= 2) <=  -MAX_SCROLL_Y ) scroll_y += 1;}
    else{
        if( (scroll_x += 2) >=  MAX_SCROLL_X )  scroll_x -= 1;}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void updateScore(a)
// redibuja la puntuacion
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void updateScore( u8 num_player, u16 score, u8 mode ){

    u8 pos = 0;

    if( score < 10 )
        pos = 0;
    else if( score >= 10 && score < 100 )
        pos = 1;
    else if( score >= 100 && score < 1000 )
        pos = 2;
    else if( score >= 1000 && score < 10000 )
        pos = 3;
    else
        pos = 4;

    // ingame
    if( mode == 0){
        if( num_player < 2)
            VDP_drawNumber(score, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 8+num_player*10-pos, 1);
        else
            VDP_drawNumber(score, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 29+(num_player-2)*10-pos, 1);
    }
    // pantalla de resultados
    else
        VDP_drawNumber(score, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 23-pos, 7+4*num_player);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void changeAnimeSprites(){
// cambia la animacion de los sprites
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void changeAnimSprites( u8 anim_frame, u8 num_players ){

    u8 a, b, c;

    // para todos los jugadores
    for( a = 0; a < num_players; a++ ){

        // el jugador está activo, animo el sprite
        if( player[a].state > 0 && player[a].state < 5 ){
            if( player[a].dir == 0 )
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 1, 1, tile_player_pos+a*4+anim_frame*16);
            else if( player[a].dir == 1 )
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_player_pos+a*4+anim_frame*16+2);
            else if( player[a].dir == 2 )
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_player_pos+a*4+anim_frame*16);
            else
                player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 1, 1, tile_player_pos+a*4+anim_frame*16+2);
        }
        // explosion
        else{

            // animo el sprite de la explosion
            player[a].frame++;
            player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL2, 1, 1, 1, tile_player_explosion_pos+player[a].frame*4);

            // si la explosion ha terminado, reactivamos al jugador
            if( player[a].frame == max_num_frame_explosion ){

                // borramos la estela del jugador del mapa de la zona de juego
                for( b = 1; b < screen_width_tiles-1; b++ ){
                    for( c = 3; c < screen_height_tiles-1; c++ ){

                        if( grid[b][c] >= a*4+1 && grid[b][c] <= a*4+4 ){
                            grid[b][c] = 0;

                            VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos-1), b, c );
                        }
                    }
                }

                // reinicio al jugador
                player[a].posx = FIX32(player[a].init_posx);
                player[a].posy = FIX32(player[a].init_posy);

                player[a].sprite.size = SPRITE_SIZE(1, 2);
                player[a].speed = FIX32(speed_1);
                player[a].joy_press = player[a].frame = 0;
                player[a].state = 1;

                // las direcciones iniciales dependen del numero de jugadores
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
            }

            // debemos refrescar el sprite pq el otro punto donde ocurre
            // (updatePosPlayers) no se ejecuta en el estado explosion
            VDP_setSpriteP(players_sprite_index+a, &player[a].sprite);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void changeAnimeSprites( u8 num_players ){
// cambia la animacion de los sprites
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void updatePosPlayers( u8 num_players ){

    u16 a, x1, x2, y1, y2;
    u8 temp;

    // para todos los jugadores
    for( a = 0; a < num_players; a++ ){ // num_players

        // si los jugadores estan activos
        if( player[a].state > 0 && player[a].state < 5 ){

            // guardamos la posicion inicial
            x1 = fix32ToInt(player[a].posx)>>3;
            y1 = fix32ToInt(player[a].posy)>>3;

            // entonces modificamos la posicion
            if( player[a].dir == 0 )
                player[a].posy = fix32Sub(player[a].posy, player[a].speed );
            else if( player[a].dir == 1 )
                player[a].posx = fix32Add(player[a].posx, player[a].speed );
            else if( player[a].dir == 2 )
                player[a].posy = fix32Add(player[a].posy, player[a].speed );
            else if( player[a].dir == 3 )
                player[a].posx = fix32Sub(player[a].posx, player[a].speed );

            // recolocamos le sprite en la posicion nueva
            if( player[a].dir == 0 || player[a].dir == 2 ){
                player[a].sprite.posx = fix32ToInt(player[a].posx)-4;
                player[a].sprite.posy = fix32ToInt(player[a].posy)-8;
            }
            else{
                player[a].sprite.posx = fix32ToInt(player[a].posx)-8;
                player[a].sprite.posy = fix32ToInt(player[a].posy)-4;
            }

            VDP_setSpriteP(players_sprite_index+a, &player[a].sprite);

            // guardamos la posicion modificada
            x2 = fix32ToInt(player[a].posx)>>3;
            y2 = fix32ToInt(player[a].posy)>>3;
            temp = grid[x2][y2];

            // si el jugador ha cambiado de tile
            if( x1 != x2 || y1 != y2 ){

                // si el nuevo tile no es vacio o el jugador ha llegado a los bordes, el jugador explota
                if( temp != 0 || x2 == 1 || x2 == screen_width_tiles-1 || y2 == 2 || y2 == screen_height_tiles-1){

                    // camobiamos su estado
                    player[a].state = 5;

                    // correciones para las explosion
                    player[a].sprite.posx -= 4;
                    if( player[a].dir == 1 || player[a].dir == 3 )
                        player[a].sprite.posy -= 4;

                    player[a].frame = 0;
                    player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL2, 1, 1, 1, tile_player_explosion_pos);
                    player[a].sprite.size = SPRITE_SIZE(2, 2);

                    VDP_setSpriteP(players_sprite_index+a, &player[a].sprite);

                    // incrementamos la puntuacion del jugador con el que hemos chocado y no vale consigo mismo ni con las paredes
                    if( x2 != 1 && x2 != screen_width_tiles-1 && y2 != 2 && y2 != screen_height_tiles-1 ){
                        if( temp < 5 && a != 0 ){
                            player[0].score += score_hit;
                            updateScore(0, player[0].score, 0);
                        }
                        else if( temp >= 5 && temp < 10 && a != 1 ){
                            player[1].score += score_hit;
                            updateScore(1, player[1].score, 0);
                        }
                        else if( temp >= 10 && temp < 14 && a != 2 ){
                            player[2].score += score_hit;
                            updateScore(2, player[2].score, 0);
                        }
                        else if( a != 3 ){
                            player[3].score += score_hit;
                            updateScore(3, player[3].score, 0);
                        }
                    }

                    psgFxPlay(3);
                }
                else{

                    // actualiza la zona de juego
                    if( player[a].dir == 0 || player[a].dir == 2 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a), x2, y2 );

                        // rellenamos la casilla
                        grid[x2][y2] = a*4+1;
                    }
                    else{
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+1+4*a), x2, y2 );

                        // rellenamos la casilla
                        grid[x2][y2] = a*4+2;
                    }

                    // incremento la puntuacion y la dibujo
                    player[a].score += score_paint;
                    updateScore(a, player[a].score, 0);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void changeDirPlayers(){
// cambia la DIRECCION de los jugadores con el pad
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void changeDirPlayers( u8 num_players, u8 num_human_players ){

    unsigned int keys_pad1 = 0, keys_pad2 = 0;

    // lee el pad
    keys_pad1 = get_pad(0) & SEGA_CTRL_BUTTONS;

    // jugador 1
    if( player[0].state > 0 && player[0].state < 5 ){
        if(keys_pad1 & SEGA_CTRL_LEFT && !player[0].joy_press ){

            // cambiamos la direccion
            if( player[0].new_dir > 0 )
                player[0].new_dir--;
            else
                player[0].new_dir = 3;

            player[0].joy_press = 1;
        }
        else if(keys_pad1 & SEGA_CTRL_RIGHT && !player[0].joy_press ){

            player[0].new_dir++;
            if( player[0].new_dir == 4 )
                player[0].new_dir = 0;

            player[0].joy_press = 1;
        }
        else if( !(keys_pad1 & SEGA_CTRL_RIGHT) && !(keys_pad1 & SEGA_CTRL_LEFT) )
            player[0].joy_press = 0;
    }

    // hay dos jugadores humanos o mas
    if( num_human_players > 1 ){

        // lee el pad 2
        keys_pad2 = get_pad(1) & SEGA_CTRL_BUTTONS;

        // hay 2 jugadores solo
        if( num_human_players == 2 ){

            // hacemos lo mismo con el pad 2
            if( player[1].state > 0 && player[1].state < 5 ){
                if(keys_pad2 & SEGA_CTRL_LEFT && !player[1].joy_press ){

                    if( player[1].new_dir > 0 )
                        player[1].new_dir--;
                    else
                        player[1].new_dir = 3;

                    player[1].joy_press = 1;
                }
                else if(keys_pad2 & SEGA_CTRL_RIGHT && !player[1].joy_press ){

                    player[1].new_dir++;
                    if( player[1].new_dir == 4 )
                        player[1].new_dir = 0;

                    player[1].joy_press = 1;
                }
                else if( !(keys_pad2 & SEGA_CTRL_RIGHT) && !(keys_pad2 & SEGA_CTRL_LEFT) )
                    player[1].joy_press = 0;
            }
        }
        // 3 o 4
        else{

            if( player[1].state > 0 && player[1].state < 5 ){
                if(keys_pad1 & SEGA_CTRL_A && !player[1].joy_press ){

                    if( player[1].new_dir > 0 )
                        player[1].new_dir--;
                    else
                        player[1].new_dir = 3;

                    player[1].joy_press = 1;
                }
                else if(keys_pad1 & SEGA_CTRL_B && !player[1].joy_press ){

                    player[1].new_dir++;
                    if( player[1].new_dir == 4 )
                        player[1].new_dir = 0;

                    player[1].joy_press = 1;
                }
                else if( !(keys_pad1 & SEGA_CTRL_A) && !(keys_pad1 & SEGA_CTRL_B) )
                    player[1].joy_press = 0;
            }

            if( player[2].state > 0 && player[2].state < 5 ){
                if(keys_pad2 & SEGA_CTRL_LEFT && !player[2].joy_press ){

                   if( player[2].new_dir > 0 )
                        player[2].new_dir--;
                    else
                        player[2].new_dir = 3;

                    player[2].joy_press = 1;
                }
                else if(keys_pad2 & SEGA_CTRL_RIGHT && !player[2].joy_press ){

                    player[2].new_dir++;
                    if( player[2].new_dir == 4 )
                        player[2].new_dir = 0;

                    player[2].joy_press = 1;
                }
                else if( !(keys_pad2 & SEGA_CTRL_RIGHT) && !(keys_pad2 & SEGA_CTRL_LEFT) )
                    player[2].joy_press = 0;
            }

            if( player[3].state > 0 && player[3].state < 5 ){
                if(keys_pad2 & SEGA_CTRL_A && !player[3].joy_press ){

                    if( player[3].new_dir > 0 )
                        player[3].new_dir--;
                    else
                        player[3].new_dir = 3;

                    player[3].joy_press = 1;
                }
                else if(keys_pad2 & SEGA_CTRL_B && !player[3].joy_press ){

                    player[3].new_dir++;
                    if( player[3].new_dir == 4 )
                        player[3].new_dir = 0;

                    player[3].joy_press = 1;
                }
                else if( !(keys_pad2 & SEGA_CTRL_A) && !(keys_pad2 & SEGA_CTRL_B) )
                    player[3].joy_press = 0;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void changeDirPlayers(){
// actualizo la direccion de los jugadores en las intersecciones
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void updateDirPlayers( u8 num_players ){

    u16 temp_x, temp_y;
    u8 a;

    // para todos los jugadores
    for( a = 0; a < num_players; a++ ){

        // si el jugador está activo
        if( player[a].state > 0 && player[a].state < 5 ){

            // si hemos cambiado la direccion
            if( player[a].new_dir != player[a].dir ){

                temp_x = (fix32ToInt(player[a].posx)>>3);
                temp_y = (fix32ToInt(player[a].posy)>>3);

                // arriba/abajo cambia a derecha/izquierda solo en las intersecciones
                if(( player[a].dir == 0 || player[a].dir == 2 ) && (fix32ToInt(player[0].posy)%8 == 0 )){

                    // rellenamos la casilla
                    grid[temp_x][temp_y] = 4*a+1;

                    // correcciones chapuceicas (esquinas)
                    if( player[a].dir == 0 && player[a].new_dir == 1 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a+3), temp_x, temp_y );
                        grid[temp_x][temp_y] = a*4+4;
                    }
                    else if( player[a].dir == 2 && player[a].new_dir == 1 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a+1), temp_x, temp_y );
                        grid[temp_x][temp_y] = a*4+2;
                    }
                    else if( player[a].dir == 2 && player[a].new_dir == 3 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a+2), temp_x, temp_y );
                        grid[temp_x][temp_y] = a*4+3;
                    }

                    // cambiamos la direccion por la nueva
                    player[a].dir = player[a].new_dir;

                    // cambio el sprite
                    player[a].sprite.size = SPRITE_SIZE(2, 1);

                    if( player[a].dir == 1 )
                        player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_player_pos+a*4+2);
                    else
                        player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 1, 1, tile_player_pos+a*4+2);

                    // autocorrecion de la posicion para ajustarse al tile
                    player[a].posx = FIX32(temp_x<<3);
                    player[a].posy = FIX32(temp_y<<3);

                    //SND_loadPcm_XGM(0);
                    psgFxPlay(8);
                }
                else if(( player[a].dir == 1 || player[a].dir == 3 ) && (fix32ToInt(player[0].posx)%8 == 0 )){

                    // rellenamos la casilla
                    grid[temp_x][temp_y] = 4*a+2;

                    // correcciones chapuceicas (esquinas)
                    if( player[a].dir == 1 && player[a].new_dir == 0 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a+2), temp_x, temp_y );
                        grid[temp_x][temp_y] = a*4+3;
                    }
                    else if( player[a].dir == 1 && player[a].new_dir == 2 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a), temp_x, temp_y );
                        grid[temp_x][temp_y] = a*4+1;
                    }
                    else if( player[a].dir == 3 && player[a].new_dir == 2 ){
                        VDP_setTileMap( APLAN, TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_grid_pos+4*a+3), temp_x, temp_y );
                        grid[temp_x][temp_y] = a*4+4;
                    }

                    // cambiamos la direccion por la nueva
                    player[a].dir = player[a].new_dir;

                    // cambio el sprite
                    player[a].sprite.size = SPRITE_SIZE(1, 2);

                    if( player[a].dir == 2 )
                        player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 0, 0, tile_player_pos+a*4);
                    else
                        player[a].sprite.tile_attr = TILE_ATTR_FULL(PAL1, 1, 1, 1, tile_player_pos+a*4);

                    // autocorrecion de la posicion para ajustarse al tile
                    player[a].posx = FIX32(temp_x<<3);
                    player[a].posy = FIX32(temp_y<<3);

                    //SND_loadPcm_XGM(0);
                    psgFxPlay(8);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void updatePosPrograms( num_players, num_human_players ){
// IA de los enemigos
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void updatePosPrograms( num_players, num_human_players ){

    u8 a, b;
    u16 temp_x, temp_y;

    // para todos los enemigos
    for( a = num_human_players; a < num_players; a++ ){

        temp_x = (fix32ToInt(player[a].posx)>>3);
        temp_y = (fix32ToInt(player[a].posy)>>3);

        b = random() % 100;

        // cada x tiempo random establecido cambio la direccion
        player[a].ticks++;
        if( player[a].ticks  == 60 ){
            player[a].ticks = 0;
            player[a].segs++;
            if( player[a].segs >= player[a].segs_obj ){
                player[a].segs_obj = random() % 8;
                player[a].segs = 0;

                // arriba
                if( player[a].dir == 0 && player[a].new_dir == 0 ){

                    if( b > 50 ){
                        if( grid[temp_x-1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                    }
                    else{
                        if( grid[temp_x-1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                    }
                }
                // derecha
                else if( player[a].dir == 1 && player[a].new_dir == 1 ){

                    if( b > 50 ){
                        if( grid[temp_x][temp_y-1] == 0 && temp_y > 4  )
                            player[a].new_dir = 0;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                    }
                    else{
                        if( grid[temp_x][temp_y-1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y > 4 )
                            player[a].new_dir = 0;
                    }
                }
                // abajo
                else if( player[a].dir == 2 && player[a].new_dir == 2 ){

                    if( b < 50 ){
                        if( grid[temp_x-1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                    }
                    else{
                        if( grid[temp_x-1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                    }
                }
                // izquierda
                else if( player[a].dir == 3 && player[a].new_dir == 3 ){
                    if( b < 50 ){
                        if( grid[temp_x][temp_y-1] == 0 && temp_y > 4 )
                            player[a].new_dir = 0;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                    }
                    else{
                        if( grid[temp_x][temp_y-1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y > 4 )
                            player[a].new_dir = 0;
                    }
                }
            }
        }
        // cambio de direccion obligatorio al llegar a una pared o los extremos del mapa
        else{

            // arriba
            if( player[a].dir == 0 && player[a].new_dir == 0 ){

                if( grid[temp_x][temp_y-1] != 0 || temp_y == 4 ){

                    if( b > 50 ){
                        if( grid[temp_x-1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                    }
                    else{
                        if( grid[temp_x-1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                    }
                }
            }
            // derecha
            else if( player[a].dir == 1 && player[a].new_dir == 1 ){

                if( grid[temp_x+1][temp_y] != 0 || temp_x == screen_width_tiles-2 ){

                    if( b > 50 ){
                        if( grid[temp_x][temp_y-1] == 0 && temp_y > 4 )
                                player[a].new_dir = 0;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                    }
                    else{
                        if( grid[temp_x][temp_y-1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y > 4 )
                            player[a].new_dir = 0;
                    }
                }
            }
            // abajo
            else if( player[a].dir == 2 && player[a].new_dir == 2 ){

                if( grid[temp_x][temp_y+1] != 0 || temp_y == screen_height_tiles-2 ){

                    if( b < 50 ){
                        if( grid[temp_x-1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                    }
                    else{
                        if( grid[temp_x-1][temp_y] == 0 && temp_x < screen_width_tiles-2 )
                            player[a].new_dir = 1;
                        else if( grid[temp_x+1][temp_y] == 0 && temp_x > 2 )
                            player[a].new_dir = 3;
                    }
                }
            }
            // izquierda
            else if( player[a].dir == 3 && player[a].new_dir == 3 ){

                if( grid[temp_x-1][temp_y] != 0 || temp_x == 2 ){

                    if( b < 50 ){
                        if( grid[temp_x][temp_y-1] == 0 && temp_y > 4 )
                            player[a].new_dir = 0;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                    }
                    else{
                        if( grid[temp_x][temp_y-1] == 0 && temp_y < screen_height_tiles-2 )
                            player[a].new_dir = 2;
                        else if( grid[temp_x][temp_y+1] == 0 && temp_y > 4 )
                            player[a].new_dir = 0;
                    }
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void gameLoop( u8 num_players, u8 num_human_players )
// bucle del juego
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gameLoop( u8 num_players, u8 num_human_players ){

    u8 cnt = 0, anim_frame = 0;
    u8 ticks = 0, segs = 0;

    // para el post juego
    u16 score_temp[4];
    unsigned int keys_pad1 = 0;
    u8 scroll = 0, pressed = 1;
    const char *str = {"END OF LINE"};
    u8 len = 0, letter = 0;
    char str_temp[12];

    while(1){

        // cambio la animacion de los jugadores
        cnt++;
        if( cnt == anim_speed ){
            anim_frame++;
            cnt = 0;

            if( anim_frame == max_num_frames )
                anim_frame = 0;

            changeAnimSprites( anim_frame, num_players );
        }

        // cambio la direccion de los jugadores con el pad
        changeDirPlayers( num_players, num_human_players );

        // actualizo la direccion de los jugadores cuando se cambia de tile
        updateDirPlayers( num_players);

        // muevo los jugadores por la pantalla
        updatePosPlayers( num_players );

        // IA de los enemigos
        updatePosPrograms( num_players, num_human_players );

        // actualiza el starfield
        updateStars3D( num_players );

        ticks++;
        if( ticks == 60 ){

            ticks = 0;
            segs++;

            if( segs == gameTime ) // gameTime
                break;
        }


        //VDP_drawText("                                    ", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 0, 0);
        //VDP_drawText("                                    ", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 0, 1);

        /*VDP_drawNumber( fix32ToInt(player[0].posx), BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 0, 0);
        VDP_drawNumber( fix32ToInt(player[0].posy), BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 4, 0);
        VDP_drawNumber(player[0].sprite.posx, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 0, 1);
        VDP_drawNumber( player[0].sprite.posy, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 4, 1);

        VDP_drawNumber( player[0].dir, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 8, 0);
        VDP_drawNumber( player[0].new_dir, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 8, 1);

        VDP_drawNumber( fix32ToInt(player[0].posx)>>3, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 10, 0);
        VDP_drawNumber( fix32ToInt(player[0].posy)>>3, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 10, 1);

        VDP_drawNumber( fix32ToInt(player[1].posx), BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 20, 0);
        VDP_drawNumber( fix32ToInt(player[1].posy), BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 24, 0);
        VDP_drawNumber(player[1].sprite.posx, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 20, 1);
        VDP_drawNumber( player[1].sprite.posy, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 24, 1);

        VDP_drawNumber( player[1].dir, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 28, 0);
        VDP_drawNumber( player[1].new_dir, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 28, 1);

        VDP_drawNumber( fix32ToInt(player[1].posx)>>3, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 30, 0);
        VDP_drawNumber( fix32ToInt(player[1].posy)>>3, BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 30, 1);*/

        psgFxFrame();
        VDP_waitVSync();
        VDP_updateSprites();
    }

    // fade out de paleta
    VDP_initFading3(0, 15, bg_00_pal, palette_black, 16, 31, sprites_00_pal, palette_black, 32, 47, title_font_pal, palette_black, 30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading3()){

        cnt++;
        if( cnt == anim_speed ){
            anim_frame++;
            cnt = 0;

            if( anim_frame == max_num_frames )
                anim_frame = 0;

            changeAnimSprites( anim_frame, num_players );
        }
        changeDirPlayers( num_players, num_human_players );
        updateDirPlayers( num_players);
        updatePosPlayers( num_players );
        updateStars3D( num_players );
        psgFxFrame();
        VDP_waitVSync();
    }

    // cambia la musica
    if( !cd_unit )
        SND_loadSong_XGM(3);
    else{
        // reproduce la pista 8
        write_word(0xA12010, 14);
        write_byte(0xA12012, 0x01); // reproduce la pista y repite
        wait_do_cmd('P');           // instruccion PlayTrack
        wait_cmd_ack();
        write_byte(0xA1200E, 0x00); // confirmacion recibida del resultado de la instruccion
    }

    // reseteo las capas, los sprites
    VDP_resetSprites();
    VDP_updateSprites();

    //clear_screen();
    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    VDP_drawText("!\"#", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 6);
    VDP_drawText("$%&  00000", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 7);

    VDP_drawText("'()", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 10);
    VDP_drawText("*+,  00000", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 11);

    VDP_drawText("-./", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 14);
    VDP_drawText(":;<  00000", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 15);

    VDP_drawText("=>?", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 18);
    VDP_drawText("[\\]  00000", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 14, 19);

    // cargo los tiles del fondo
    VDP_loadTileData(menu_bg_tiles, 1, menu_bg_tiles_size, 1);

    // dibuja el mapa
    VDP_fillTileMap( APLAN, menu_bg_map, 0, 0, menu_bg_tile_x_size, menu_bg_tile_y_size, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1));  // plano, mapa, x, y, ancho, alto, atributos de tile

    for( cnt = 0; cnt < 4; cnt++ )
        score_temp[cnt] = 0;

    // fade in de paleta
    VDP_initFading3(0, 15, palette_black, bg_00_pal, 16, 31, palette_black, title_font_pal, 32, 47, palette_black, title_font_pal,30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading3()){
        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);
        psgFxFrame();
        VDP_waitVSync();
    }

    // incremento de las puntuaciones
    segs = 0;
    while(1){

        // lee el pad
        keys_pad1 = get_pad(0) & SEGA_CTRL_BUTTONS;

        if(keys_pad1 & SEGA_CTRL_A)
            break;

        ticks = 0;
        for( cnt = 0; cnt < num_players; cnt++ ){
            if( score_temp[cnt] < player[cnt].score ){

                score_temp[cnt] += 10;
                updateScore( cnt, score_temp[cnt], 1 );
                ticks = 1;
            }
        }
        if( ticks != 0 )
            psgFxPlay(10);
        else{
            segs++;
            if( segs == 120 )
                break;
        }

        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        psgFxFrame();
        VDP_waitVSync();
    }

    for( cnt = 0; cnt < num_players; cnt++ )
        updateScore( cnt, player[cnt].score, 1 );

    psgFxPlay(0);

    // calcula la maxima puntuacion
    if( num_players < 3 ){

        if( player[0].score > player[1].score )
            cnt = 0;
        else
            cnt = 1;
    }
    else{
        if( player[0].score > player[1].score && player[0].score > player[2].score && player[0].score > player[3].score )
            cnt = 0;
        else{
            if( player[1].score > player[0].score && player[1].score > player[2].score && player[1].score > player[3].score )
                cnt = 1;
            else{
                if( player[2].score > player[0].score && player[2].score > player[1].score && player[2].score > player[3].score )
                    cnt = 2;
                else
                    cnt = 3;
            }
        }
    }

    if( cnt < num_human_players )
        VDP_drawText("USER HAS FREED THE PROGRAM", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 7, 25);
    else{
        VDP_fillTileMap( BPLAN, mcp_map, 28, 7+4*cnt-6, 3, 5, TILE_ATTR_FULL(PAL3, 1, 0, 0, mcp_tiles_pos));  // plano, mapa, x, y, ancho, alto, atributos de tile
        VDP_drawText("MCP HAS DEFEATED THE USERS", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 7, 25);
    }

    segs = 0;
    while(1){

        // lee el pad
        keys_pad1 = get_pad(0) & SEGA_CTRL_BUTTONS;

        if( keys_pad1 & SEGA_CTRL_A && pressed == 0 ){
            break;
        }
        else if(!(keys_pad1 & SEGA_CTRL_A) && pressed == 1 )
            pressed = 0;

        segs++;
        if( segs < 10 )
            VDP_drawText("VICTORY", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 26, 7+4*cnt);
        else if( segs >= 10 && segs < 20 )
            VDP_drawText("       ", BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font), 26, 7+4*cnt);
        else if( segs == 30 )
            segs = 0;

        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        psgFxFrame();
        VDP_waitVSync();
    }

    // fade out de paleta
    VDP_initFading4(0, 15, bg_00_pal, palette_black, 16, 31, title_font_pal, palette_black, 32, 47, title_font_pal, palette_black, 48, 63, mcp_pal, palette_black,30);    // from col, to col, pal src, pal dst, numframes);
    while(VDP_doStepFading4()){

        scroll += 1;
        VDP_setHorizontalScroll(APLAN, 0, -scroll);
        VDP_setVerticalScroll(APLAN, 0, scroll);

        psgFxFrame();
        VDP_waitVSync();
    }

    VDP_clearPlan(APLAN);
    VDP_clearPlan(BPLAN);

    VDP_setPalette((u16 *)title_font_pal, 32, 16);

    // dibujamos el texto final
    ticks = 0;
    while(1){
        if( ticks % 4 == 0 ){
            // copiamos la cadena que usaremos en la temporal
            len = strlen_(str);
            strncpy( str_temp, str, len);

            // dibuja el texto letra a letra
            if( letter < len ){
                VDP_setTileMap(BPLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, pos_vram_font+(int)str_temp[letter])-32, letter+14, 14);
                psgFxPlay(2);
            }
            else
                break;

            letter++;
        }
        ticks++;

        psgFxFrame();
        VDP_waitVSync();
    }

    for( ticks = 0; ticks < 150; ticks++ ){
        psgFxFrame();
        VDP_waitVSync();
    }

    VDP_setPalette((u16 *)palette_black, 32, 16);

    // calcula la maxima puntuacion para la pantalla de records (sram)
    if( num_human_players == 1 && cnt == 0 )
        score = player[0].score;
    else
        score = 0;
}

