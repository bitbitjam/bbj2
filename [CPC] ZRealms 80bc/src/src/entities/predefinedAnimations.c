//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine 
//  Copyright (C) 2015 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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

#include "entities.h"
#include "../sprites/sprites.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////
//////  PREDEFINED ANIMATIONS
//////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//
// All the animation frames to be used in this example
//
const TAnimFrame g_allAnimFrames[6] = {
   { g_sprite1,   4, 16,  2 }, // 0 | walk-left
   { g_sprite2,   4, 16,  2 }, // 1 |
   { g_sprite3,   4, 16,  2 }, // 2   | walk-right
   { g_sprite4,   4, 16,  2 }, // 3   |
   { g_sprite0,   4, 16,  1 }  // 4 | static
};

// Use a define for convenience (making name of the constant shorter)
#define AF g_allAnimFrames

//
// All complete animations used in this example (NULL terminated, to know the end of the array)
//
TAnimFrame*  const g_walkLeft[3]  = { &AF[0], &AF[1],  0 };
TAnimFrame*  const g_walkRight[3] = { &AF[2], &AF[3],  0 };
TAnimFrame*  const g_static[2]    = { &AF[5],  0 };

//
// Vector containing references to all animations, ordered by states and sides
//
/*
TAnimFrame** const g_anim[es_NUMSTATUSES][s_NUMSIDES] = {
   {  0, 0  },                   // STATE 0 = es_static: No animations
   { g_walkLeft, g_walkRight },  // STATE 1 = es_walk
   { g_jumpLeft, g_jumpRight },  // STATE 2 = es_jump
   { g_hitLeft,  g_hitRight  }   // STATE 3 = es_hit
};
*/
// Macro can be undefined, as it is not required anymore
#undef AF
