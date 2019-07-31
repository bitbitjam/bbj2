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

#include "tilemaps.h"

////////////////////////////////////////////////////////////////
// Available Tilemaps
//
const u8 g_tilemaps[G_NUMTILEMAPS][G_TILEMAP_SIZE] = {
   { //--------------------------------- 00
      #include "tilemaps/screen00.csv" 
   },
   { //--------------------------------- 01
      #include "tilemaps/screen01.csv" 
   },
   { //--------------------------------- 02
      #include "tilemaps/screen02.csv" 
   },
   { //--------------------------------- 03
      #include "tilemaps/screen03.csv" 
   },
   { //--------------------------------- 04
      #include "tilemaps/screen04.csv" 
   },
   { //--------------------------------- 05
      #include "tilemaps/screen05.csv" 
   },
   { //--------------------------------- 06
      #include "tilemaps/screen06.csv" 
   },
   { //--------------------------------- 07
      #include "tilemaps/screen07.csv" 
   },
   { //--------------------------------- 08
      #include "tilemaps/screen08.csv" 
   },
   { //--------------------------------- 09
      #include "tilemaps/screen09.csv" 
   }
};
