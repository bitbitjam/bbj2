// intro
#define delayBetweenMessages    110
#define num_screens_intro       5
#define num_strings_intro       13

#define pos_x_strg1     7
#define pos_y_strg1     23

#define pos_x_strg2     9
#define pos_y_strg2     21
#define pos_x_strg3     7
#define pos_y_strg3     23
#define pos_x_strg4     9
#define pos_y_strg4     25

#define pos_x_strg5     5
#define pos_y_strg5     21
#define pos_x_strg6     9
#define pos_y_strg6     23
#define pos_x_strg7     7
#define pos_y_strg7     25

#define pos_x_strg8     7
#define pos_y_strg8     21
#define pos_x_strg9     11
#define pos_y_strg9     23
#define pos_x_strg10    5
#define pos_y_strg10    25

#define pos_x_strg11    6
#define pos_y_strg11    21
#define pos_x_strg12    4
#define pos_y_strg12    23
#define pos_x_strg13    3
#define pos_y_strg13    25


#define tam_map_h_tiles         40
#define tam_map_v_tiles         27

typedef struct
{
	u16  *map;
	u32  *data;
	u16  *pal;
	u16  size;
}
INTRO;

u8 introScreen();
void draw1985Logo();
u8 recordsScreen( u8 mode );
u8 titleScreen();
void creditsScreen();
void startupScreen();
void menuScreen( u8 *num_players, u8 *num_human_players );

//  para el efecto de scroll por persiana de Doragasu
u16 vscroll_a;
u8 start_line;

// sprite de fondo del menu
SpriteDef sprite_menu[44];
