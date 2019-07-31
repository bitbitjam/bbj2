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

#include "sprites/screens.h"
#include "helpers.h"
#include "game.h"
#include <cpctelera.h>

////////////////////////////////////////////////////////////////////////////////////////
// This controls de menu
//
u8 menuloop() {
   cpct_TEasyTilemap* menuscr = g_screens + 2;
   u8* pvmem;

   // Draw Menu Screen
   cpct_clearScreen(0x00);
   cpct_etm_redrawTileBox(menuscr, 0, 0, menuscr->map_width, menuscr->map_height);
   pvmem = cpct_getScreenPtr(SCR_VMEM_0, 18, 172);
   cpct_drawStringM0("PRESS SPACE", pvmem, 3, 0);
   pvmem = cpct_getScreenPtr(SCR_VMEM_0, 23, 188);
   cpct_drawStringM0("TO START", pvmem, 4, 0);
   
   return wait4Key(Key_Space);
}

////////////////////////////////////////////////////////////////////////////////////////
// Main entry point of the application
//
void main(void) {
   __asm__ ("ld sp, #0x8000");
   initial_setup();
   
   while (1) {    
      gameloop( menuloop() );
   }
}
