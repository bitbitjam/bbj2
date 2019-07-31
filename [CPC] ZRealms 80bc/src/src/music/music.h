//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of the game: ZRealms 80 b.c.
//  Copyright (C) 2015 ronaldo  / Cheesetea (@FranGallegoBR)
//  Copyright (C) 2015 Predator / Cheesetea (@Hec_Linares)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef MUSIC_H
#define MUSIC_H

#include <cpctelera.h>

// Song locations in memory
#define CT2_SONG_LOCATION  0x040
#define cpct_transparentMaskTableM0_address 0x400
#define SPRITES_LOCATION 0x500

       
// CT2 song by Predator / Cheesetea
__at(CT2_SONG_LOCATION) extern const u8 g_ct2_song[756];

__at(SPRITES_LOCATION) extern const u8 g_sprite0[4*16];
extern const u8 g_sprite1[4*16];
extern const u8 g_sprite2[4*16];
extern const u8 g_sprite3[4*16];
extern const u8 g_sprite4[4*16];
extern const u8 g_sprite5[4*16];
extern const u8 g_sprite6[4*16];
extern const u8 g_sprite7[4*16];
extern const u8 g_sprite8[4*16];
extern const u8 g_sprite9[4*16];

#endif
