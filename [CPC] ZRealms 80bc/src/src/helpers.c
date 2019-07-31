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

#include "helpers.h"
#include "sprites/tileset.h"

//
// Wait for n vsyncs
//
void waitNVSYNCs(u8 n) {
   do {
      cpct_waitVSYNC();
      if (--n) {
         __asm__("halt");
         __asm__("halt");
      }
   } while(n);
}

//
// Wait 'till the user presses a given key (with music playing?)
//
u8 wait4Key(cpct_keyID key) {
   u8 cycles = 0;
   
   // Wait for Keypress
   do {
      ++cycles;
      waitNVSYNCs(2);
      cpct_scanKeyboard_f();
   } while ( !cpct_isKeyPressed(key) );

   return cycles;
}

//
// Initial setup of the game
//
void initial_setup() {
   const u8 palette[16] = { 0x17, 0x04, 0x15, 0x14, 0x18, 0x1D, 0x16, 0x06, 
                            0x1E, 0x00, 0x1F, 0x0F, 0x1A, 0x19, 0x0A, 0x0B };

   cpct_disableFirmware();
   cpct_setVideoMode(0);
   cpct_setBorder(0x04);
   //cpct_setBorder(0x17);
   cpct_etm_setTileset(g_tileset);
   cpct_setPalette(palette, 16);

   // Clear backbuffer
   cpct_memset(0x8000, 0x00, 0x4000);
}
