#ifndef _GAME_H_
#define _GAME_H_

////////////////////////////////////////////////////////////////////
// defines
#define max_num_players             4
#define players_sprite_index        0

#define speed_0                     0.25
#define speed_1                     0.75 // 0.75
#define speed_2                     1.5

#define score_paint                 10
#define score_hit                   100

#define gameTime                    180

// animacion sprites
#define anim_speed                  3
#define anim_speed_explosion        1
#define max_num_frames              4
#define max_num_frame_explosion     7

// starfield
#define MAX_Z 64
#define MAX_TYPES 3
#define MAX_STARS 64
#define MAX_SCROLL_X 48
#define MAX_SCROLL_Y 66
#define ORIGIN_X 160    // (SCREEN_W / 2)
#define ORIGIN_Y 128    // (SCREEN_H / 2) 112

////////////////////////////////////////////////////////////////////
// prototipos
void initStarfield3D();
void updateStars3D( u8 num_players );
void gameLoop( u8 num_players, u8 num_human_players );

////////////////////////////////////////////////////////////////////
// estructuras del juego

// jugador
typedef struct{
    fix32 posx, posy;	        // posicion x, y en punto fijo

    u16 init_posx, init_posy;   // posicion inicial x, y en pixels

    u16 score;

    u8 state;               // estado, 0 inactiva / 1 activa
    u8 dir;                 // direccion actual, 0 arriba, 1 derecha, 2 abajo, 3 izquierda
    u8 new_dir;             // nueva direccion a tomar
    fix32 speed;            // velocidad

    u8 segs;
    u8 ticks;
    u8 segs_obj;

    u8 joy_press;           // si está pulsado el pad

	// sprite
	u8 sprite_num;          // numero del sprite
	u16 tile_pos;           // posicion en vram de sus tiles
	u8 frame;               // frame unico de la animacion (explosion)
    SpriteDef sprite;       // sprite


}player_struct;


typedef struct{
    u16 x;
    u16 y;
}Vect2D_u16;


typedef struct{
    Vect2D_u16  pos2D[MAX_Z];
    u8          idx_pos;
    u8          max_moves;
    SpriteDef   sprite;
}_tStar3D;

_tStar3D      tStars[MAX_STARS];



////////////////////////////////////////////////////////////////////
// globales del juego
u8 speed;
u32 score;

s16 scroll_x, scroll_y;

player_struct player[max_num_players];

u8 grid[screen_width_tiles][screen_height_tiles];

#endif
