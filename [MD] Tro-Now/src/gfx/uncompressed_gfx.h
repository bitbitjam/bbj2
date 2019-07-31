#ifndef _UNCOMPRESSED_GFX_H
#define _UNCOMPRESSED_GFX_H

////////////////////////////////////////////////////////////////////
// tamaño de los tiles
////////////////////////////////////////////////////////////////////
#define logo1985_tiles_size             98
#define font_credits_tiles_size         96
#define title_logo_tiles_size           132
#define stars_tiles_size                3

#define bg_00_tiles_size                5
#define sprites_00_tiles_size           82
#define sprites_explosion_tiles_size    28
#define mcp_tiles_size                  13

#define records_title_tiles_size        23
#define records_text_tiles_size         192
#define pos_vram_tiles_text             25

#define title_logo_tile_x_size          32
#define title_logo_tile_y_size          5

#define menu_bg_tiles_size              5
#define sprite_menu_tiles_size          16

#define menu_bg_tile_x_size             64
#define menu_bg_tile_y_size             32

#define bg_00_tile_x_size               40
#define bg_00_tile_y_size               28

///////////////////////////////////////////////////////////////////////////
// posicion en vram
#define tile_player_pos                 bg_00_tiles_size+1
#define tile_grid_pos                   tile_player_pos+64+2
#define tile_stars_pos                  tile_player_pos+sprites_00_tiles_size+1
#define tile_player_explosion_pos       tile_stars_pos+stars_tiles_size
#define mcp_tiles_pos                   tile_player_explosion_pos+sprites_explosion_tiles_size


///////////////////////////////////////////////////////////////////////////
// GFX, PALETAS y MAPAS
///////////////////////////////////////////////////////////////////////////
// logo 1985
extern const u16 logo1985_pal[16];
extern const u16 logo1985_blue_pal[16];
extern const u16 logo1985_map[16*10];
extern const u32 logo1985_tiles[logo1985_tiles_size*8];

// bios
extern const u32 bios_font_tiles[font_credits_tiles_size*8];

// creditos
extern const u16 credits_font_pal[16];
extern const u32 credits_font_tiles[font_credits_tiles_size*8];

// records
extern const u16 records_title_pal[16];
extern const u32 records_title_tiles[records_title_tiles_size * 8];
extern const u32 records_title_b_tiles[records_title_tiles_size * 8];
extern const u16 records_text_pal[16];
extern const u32 records_text_tiles[records_text_tiles_size* 8];

// titulo
extern const u16 title_logo_pal[16];
extern const u16 title_logo_map[title_logo_tile_x_size*title_logo_tile_y_size];
extern const u32 title_logo_tiles[title_logo_tiles_size*8];

extern const u16 title_font_pal[16];
extern const u32 title_font_tiles[font_credits_tiles_size * 8];
extern const u32 stars_tiles[stars_tiles_size * 8];

// menu
extern const u32 menu_bg_tiles[menu_bg_tiles_size * 8];
extern const u16 menu_bg_map[menu_bg_tile_x_size*menu_bg_tile_y_size];
extern const u32 sprite_menu_tiles[sprite_menu_tiles_size * 8];
extern const u32 sprite_menu_tiles[sprite_menu_tiles_size * 8];

// bgs
extern const u16 bg_00_pal[16];
extern const u16 bg_00_map[bg_00_tile_x_size*bg_00_tile_y_size];
extern const u32 bg_00_tiles[bg_00_tiles_size * 8];

// sprites
extern const u16 sprites_00_pal[16];
extern const u32 sprites_00_tiles[sprites_00_tiles_size*8];
extern const u32 sprites_explosion_tiles[sprites_explosion_tiles_size*8];

extern const u16 mcp_pal[16];
extern const u16 mcp_map[3*5];
extern const u32 mcp_tiles[mcp_tiles_size*8];

#endif
