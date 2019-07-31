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

#ifndef ENTITIES_H
#define ENTITIES_H

#include <cpctelera.h>

#define MAX_ENTITIES     50
#define MAX_ENTITIES_SCR 10

//////////////////////////////////////////////////////////////////////////////
// PUBLIC TYPES

//////////////////////////// ANIMATIONS
//
// Possible statuses of an animation
//
typedef enum {
   AS_null = 0, // We require this to represent a null status
   AS_play = 1, // Playing till the last frame
   AS_cycle,    // Playing continuosly
   AS_pause,    // Paused, waiting to continue
   AS_end       // Animation has ended
} TAnimStatus;

//
// Description of an animation frame
//
typedef struct {
   u8* sprite;  // Sprite associated to this frame
   u8  w, h;    // Sprite dimensions in bytes
   u8  time;    // Time that the sprite should be shown
} TAnimFrame;

//
// Describes an Animation as a secuence of sprites, controlled by time
//   Time is measured in main loop cycles
//
typedef struct {
   TAnimFrame**  frames;    // Vector containing all the frames of the animation
   u8            frame_id;  // Index of the current frame
   u8            time;      // Remaining time for this frame
   TAnimStatus   status;    // Status of the animation
} TAnimation;

//////////////////////////////ENTITIES
// Entity status bits
//
extern const u8 ES_alive;
extern const u8 ES_jumping;
extern const u8 ES_falling;
extern const u8 ES_dirty;

// Entity information
//  12 bytes
//
typedef struct {
   u8      x, y; 
   u8    nx, ny;
   i8    vx, vy;
   u8      w, h;
   u8       scr;
   u8    status;  // Status byte, with signalling bits
   u8*   sprite;
} TEntity;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//
              void initializeEntities();
                u8 thereAreFreeEntities();
cpct_TEasyTilemap* getPresentScreen();
          TEntity* createGameEntity(u8 scr, u8 x, u8 y, u8 w, u8 h, u8* sprite);
              void drawAllEntities(void *pvmem);
              void drawEntity(TEntity* e, void* pvmem);
              void eraseAllEntities(cpct_TEasyTilemap* scr);
              void eraseEntity(TEntity* e, cpct_TEasyTilemap* scr);
                u8 moveEntity(TEntity *e);
              void moveAllEntities();
              void accelerateEntity(TEntity *e, i8 ax, i8 ay);
              void checkTileCollisions(TEntity *e, cpct_TEasyTilemap* scr);

#endif