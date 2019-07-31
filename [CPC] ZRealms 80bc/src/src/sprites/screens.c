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

#include "screens.h"
#include "tilemaps.h"
#include "tileset.h"

//////////////////////////////////////////////////////////////////////////////
// Game Screens: 0 and 17 are the Castles of the kingdoms
//
//          ---------------------
//          | 0 | 1 | 2 | 3 | 4 |
//          ---------------------
//              | 5 | 6 | 7 | 8 |
//              -----------------
//              | 9 |10 |11 |12 |
//              ---------------------
//              |13 |14 |15 |16 |17 |
//              ---------------------
//
//
cpct_TEasyTilemap const g_screens[G_NUMSCREENS] = { 
   { (g_tilemaps + 0), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  0
   { (g_tilemaps + 1), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  1
   { (g_tilemaps + 2), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  2
   { (g_tilemaps + 3), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  3
   { (g_tilemaps + 4), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  4
   { (g_tilemaps + 5), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  5
   { (g_tilemaps + 6), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  6
   { (g_tilemaps + 7), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  7
   { (g_tilemaps + 8), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  8
   { (g_tilemaps + 3), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, //  9
   { (g_tilemaps + 1), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 10
   { (g_tilemaps + 4), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 11
   { (g_tilemaps + 5), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 12
   { (g_tilemaps + 8), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 13
   { (g_tilemaps + 2), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 14
   { (g_tilemaps + 7), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 15
   { (g_tilemaps + 6), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }, // 16
   { (g_tilemaps + 9), g_tileset, TILEMAP_SCR_VMEM_0, 32, 32 }  // 17

};
