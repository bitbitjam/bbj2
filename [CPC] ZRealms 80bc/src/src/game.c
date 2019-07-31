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

#include <cpctelera.h>
#include "sprites/tilemaps.h"
#include "sprites/tileset.h"
#include "sprites/sprites.h"
#include "sprites/screens.h"
#include "entities/entities.h"
#include "music/music.h"
#include "utils/utils.h"
#include "helpers.h"
#include "game.h"

//#define  STEP_BY_STEP

////////////////////////////////////////////////////////////////////////////////////////
/// MEMBER ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////////////

EGameStatus        m_gameStatus;
TEntity*           m_player;
cpct_TEasyTilemap  *m_screen;
u8                 *m_frontScr, *m_backScr;
u8                 m_frontPage,  m_backPage;
u8                 m_peace;

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// Draw Peace status
//
void drawPeaceStatus(u8* scr) {
   u8 i=m_peace >> 3;
   u8 j=32 - i;

   scr += 0x50 * 17;
   while(i--) {
      drawTile(scr, g_tile20);
      scr += 2;
   }
   while(j--) {
      drawTile(scr, g_tile52);
      scr += 2;
   }
}

////////////////////////////////////////////////////////////////////////////////////////
// Clears score status zone
//
void clearScoreZone(u8* scr) {
   u8 i;
   scr += 0x50 * 20;
   for (i=0; i < 200; i++) {
      drawTile(scr,          g_tile0);
      drawTile(scr + 0x2000, g_tile0);
      scr += 2;
   }
}

////////////////////////////////////////////////////////////////////////////////////////
// Initialize a strating game
//
void startNewGame() {
   cpct_akp_musicInit(g_ct2_song);
   initializeEntities();

   // Initial values
   m_gameStatus = GS_enter_screen;
   m_screen     = g_screens + (getRandom_u8(0) % G_NUMSCREENS);//getPresentScreen();
   m_player     = createGameEntity(0, 2, 120, 4, 16, g_sprite0);
   m_frontScr   = TILEMAP_SCR_VMEM_0;
   m_backScr    = TILEMAP_SCR_VMEM_1;
   m_frontPage  = cpct_pageC0;
   m_backPage   = cpct_page80;
   m_peace      = 128;

   // Set up score status
   clearScoreZone(SCR_VMEM_0);
   clearScoreZone(SCR_VMEM_1);
   drawPeaceStatus(m_frontScr);
   drawPeaceStatus(m_backScr);
}

////////////////////////////////////////////////////////////////////////////////////////
// Stop till keypress
//
#ifdef STEP_BY_STEP
void waitStop() {
   while(1) {
      cpct_scanKeyboard_f();

      if ( cpct_isKeyPressed(Key_Space) ) {
         cpct_waitVSYNC();
         return;
      } else if ( cpct_isKeyPressed(Key_1) )
         cpct_setVideoMemoryPage(cpct_pageC0);
      else if ( cpct_isKeyPressed(Key_2) )
         cpct_setVideoMemoryPage(cpct_page80);
   }
}
#endif
////////////////////////////////////////////////////////////////////////////////////////
// This happens only in the end :) (of the game!)
//
void gameEnd() {
   u8* pvmem;

   cpct_setVideoMemoryPage(cpct_pageC0);  
   pvmem = cpct_getScreenPtr(SCR_VMEM_0, 22, 88);
   cpct_drawStringM0("GAME OVER", pvmem, 10, 0);
   cpct_akp_stop();

   wait4Key(Key_Space);
}

////////////////////////////////////////////////////////////////////////////////////////
// Switch screen buffers
//
void switchScreenBuffers() {
   u8 *paux, aux;
   
   // Change hardware screen buffer
   cpct_setVideoMemoryPage(m_backPage);
   aux = m_frontPage; m_frontPage = m_backPage; m_backPage = aux;

   // Change memory pointers to tilemaps
   paux = m_frontScr; m_frontScr = m_backScr; m_backScr = paux;

}

////////////////////////////////////////////////////////////////////////////////////////
// Reads keyboard and updates the user controlled player
//
void updateUser() {
   // Acceleration
   i8 ax = 0, ay = 0;
  
   // Update User
   cpct_scanKeyboard_f();

   if ( cpct_isKeyPressed(Key_Esc) )
      m_gameStatus = GS_game_over;
   if ( cpct_isKeyPressed(Key_CursorLeft) )
      ax -= 1;
   if ( cpct_isKeyPressed(Key_CursorRight) )
      ax += 1;
   if (   cpct_isKeyPressed(Key_CursorUp) 
      && !(m_player->status & ES_falling) ) {
      ay -= 9;
   }

   accelerateEntity(m_player, ax, ay+1);
}

////////////////////////////////////////////////////////////////////////////////////////
// Update all the game logic
//
void updateLoop() {
   // Change status
   switch(m_gameStatus) {
      
      //// GAME INITIALIZATION
      case GS_enter_screen: {
         m_gameStatus = GS_draw_tilemap;
         break;
      }
      case GS_draw_tilemap: {
         m_gameStatus = GS_play;
         break;        
      }
      case GS_play: {
         // Erase all entities on the backbuffer
         m_screen->pscreen = m_backScr;
         eraseAllEntities(m_screen);
         updateUser();
         moveAllEntities();
         checkTileCollisions(m_player, m_screen);
         break;
      }
   }

   // Wait VSYNC
   cpct_waitVSYNC();
   cpct_akp_musicPlay();
}

////////////////////////////////////////////////////////////////////////////////////////
// Draw a new frame on the screen (in game)
//
void redrawLoop() {
   switch(m_gameStatus) {     
      case GS_play: {
         // Redraw entities from present screen
         drawAllEntities(m_backScr);
         cpct_waitVSYNC();
         switchScreenBuffers();
         break;
      }
      case GS_draw_tilemap: {
         u8 w = m_screen->map_width;
         u8 h = m_screen->map_height;

         m_screen->pscreen = m_backScr;
         cpct_etm_redrawTileBox(m_screen, 0, 0, w, h);
         m_screen->pscreen = m_frontScr;
         cpct_etm_redrawTileBox(m_screen, 0, 0, w, h);
         break;
      }
   } 
}

////////////////////////////////////////////////////////////////////////////////////////
// This is the main game loop 
//
void gameloop(u8 seed) {
   // Set the seed and initialize a new game
   setRandomSeed_u8(seed);
   startNewGame();

   // Gameloop
   while(m_gameStatus != GS_game_over) {
#ifdef STEP_BY_STEP
      waitStop();
#endif
      updateLoop();
#ifdef STEP_BY_STEP
      waitStop();
#endif
      redrawLoop();
   }

   // End of the game
   gameEnd();
}
