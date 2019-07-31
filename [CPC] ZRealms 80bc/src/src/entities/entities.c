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

// Transparency table for masked sprites
#include "entities.h"
#include "../sprites/screens.h"
#include "../music/music.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// MEMBER ATTRIBUTES
//
TEntity     m_entities[MAX_ENTITIES];
u8          m_scrEntities[MAX_ENTITIES_SCR];
TAnimation  m_animations[MAX_ENTITIES_SCR];
u8          m_numEnt;
u8          m_nextAnim;
u8          m_nextFreeEnt;
u8          m_scrID;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

const u8 ES_alive   = 0x01;
const u8 ES_jumping = 0x02;
const u8 ES_falling = 0x04;
const u8 ES_dirty   = 0x08;

//////////////////////////////////////////////////////////////////////////////
// Initialize entity engine
//
void initializeEntities() {
   // Set everything to 0
   m_numEnt = m_nextFreeEnt = m_nextAnim = m_scrID = 0;

   cpct_memset(m_entities,    0, MAX_ENTITIES     * sizeof(TEntity));
   cpct_memset(m_animations,  0, MAX_ENTITIES_SCR * sizeof(TAnimation));
   cpct_memset(m_scrEntities, 0, MAX_ENTITIES_SCR);
}

//////////////////////////////////////////////////////////////////////////////
// Check if there is space for more entities
//
u8 thereAreFreeEntities() {
   return m_numEnt < MAX_ENTITIES;
}

//////////////////////////////////////////////////////////////////////////////
// Check Entity Collisions
//   Checks if an entity is collisioning with any tile, applying corrections
//

// Tiles greater than this one are all solid
#define SOLID_TILE 192

void checkTileCollisions(TEntity *e, cpct_TEasyTilemap* scr) {
   u8 tx0, tx1, ty0, ty1;  // New tilebox
   u8 d;
   u8* ptm; 

   // Calculate new tilebox
   tx0 = e->nx >> 3;
   ty0 = e->ny >> 3;
   tx1 = (e->nx + 4*e->w - 1) >> 3;
   ty1 = (e->ny + 2*e->h - 1) >> 3;

   // CASE 1: Vertical Movement
   ptm = (u8*)scr->ptilemap + tx0;
     d = 0;

   if (e->ny <= e->y) {
      // Up
      if (ty0 != (e->y >> 3)) {
         ptm += ty0 * scr->map_width;
           d  = ty0 + 1;
      }
   } else {
      // Down
      if (ty1 != ((e->y + e->h) >> 3) ) {
         ptm += ty1 * scr->map_width;
           d  = ty0;
      }
   }

   if (e->ny != e->y)
      e->status |= ES_falling;

   if (d) {
      u8 x;

      for(x=tx0; x <= tx1; x++, ptm++) {
         // Collision! Correct Y
         if ( *ptm >= SOLID_TILE ) {
            ty0   = d;
            e->ny = d << 3;
            e->vy = 0;      // << Set y velocity to 0
            // Recalculate y
            ty0   = e->ny >> 3;
            ty1   = (e->ny + 2*e->h - 1) >> 3;
            if (d==ty0)
               e->status ^= ES_falling;
         }
      }
   }

   ptm = (u8*)scr->ptilemap + ty0 * scr->map_width;
     d = 0;

   // CASE 2: Horizontal movement
   if (e->nx <= e->x) {
      // Left
      if (tx0 != (e->x >> 3)) {
         ptm += tx0;
           d  = tx0 + 1;
      }
   } else {
      // Right
      if (tx1 != ((e->x + 2*e->w) >> 3) ) {
         ptm += tx1;        
           d  = tx0;
      }
   }
   if (d) {
      // Check new ROW collisions
      u8 y;
      for(y=ty0; y <= ty1; y++, ptm += scr->map_width) {
         // Collision! Correct X
         if ( *ptm >= SOLID_TILE ) {
            tx0   = d;
            e->nx = d << 3;
            e->vx = 0;      // << Set x velocity to 0
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
// Apply acceleration to an entity
//  MAX VELOCITY is 8, so 0xxx1000 or 1xxx1000 So we only have to crop XXXs!
//
#define MAX_VEL 0x07

void accelerateEntity(TEntity *e, i8 ax, i8 ay) {
   e->vx = (e->vx + ax);
   if (e->vx > MAX_VEL || e->vx < -MAX_VEL)
      e->vx = e->vx - ax;
   e->vy = (e->vy + ay);
   if (e->vy > MAX_VEL || e->vy < -2*MAX_VEL)
      e->vy = e->vy - ay;
}

//////////////////////////////////////////////////////////////////////////////
// Finds next free entity and set its ID for next call
//
void locateNextFreeEntity() {  
   // m_nextFreeEnt = next free, or last
   while (  m_nextFreeEnt < MAX_ENTITIES - 1 
         && m_entities[m_nextFreeEnt].status & ES_alive ) ++m_nextFreeEnt;
}

//////////////////////////////////////////////////////////////////////////////
// Return a pointer to the present screen
//
cpct_TEasyTilemap* getPresentScreen() {  
   return (g_screens + m_scrID);
}

//////////////////////////////////////////////////////////////////////////////
// Add velocity to an axis and crop [0, max]
//   Use with vel <> 0
//
u8 addVelocityToLocation(u8 pos, u8 max, i8 vel) {
   u8 newpos = pos + vel;

   if (vel > 0) {
      if (max - vel < pos)
         return max;
   } else if (newpos > pos)
      return 0;
  
   return newpos;
}

//////////////////////////////////////////////////////////////////////////////
// Moves a entity
//   Returns border hitting | 2 |
//                          |103|
//                          | 4 |
u8 moveEntity(TEntity *e) {
   u8 border = 0;

   if (e->vx || e->vy) {
      e->nx = addVelocityToLocation(e->x, 255 - 4*e->w, e->vx);
      e->ny = addVelocityToLocation(e->y, 255 - 2*e->h, e->vy);
      e->status |= ES_dirty;
/*
      if ( e->status & ES_jumping ) {
         // End of jump status
         if ( e->vy = -MAX_VEL_Y ) {
            e->
         }
         e->vy -GRAVITY_Y;
      }
   */
   }

   return border;
}

//////////////////////////////////////////////////////////////////////////////
// Moves all entities
//
void moveAllEntities() {
   u8 eid = 0;
   
   // Move all active entities in this screen
   while (eid < MAX_ENTITIES_SCR && m_scrEntities[eid]) {
      moveEntity(m_entities + m_scrEntities[eid]);
      ++eid;
   }
   // And also the player (entity 0)
   moveEntity(m_entities);
}

//////////////////////////////////////////////////////////////////////////////
// Erases a entity
//
void eraseEntity(TEntity* e, cpct_TEasyTilemap* scr) {
   if (e->status & ES_dirty) {
      // Tile starting locations and width
      u8 tx = (e->x >> 3);
      u8 ty = (e->y >> 3);
      u8 tw = (e->w >> 1) + ( (e->x & 0x07) ? 1 : 0 );
      u8 th = (e->h >> 2) + ( (e->y & 0x07) ? 1 : 0 );

      // Redraw tilebox to erase the entity
      cpct_etm_redrawTileBox(scr, tx, ty, tw, th);

      // Not dirty anymore
      e->status ^= ES_dirty;
      e->x = e->nx;
      e->y = e->ny;
   }
}

//////////////////////////////////////////////////////////////////////////////
// Erases all changed entities and updates locations
//
void eraseAllEntities(cpct_TEasyTilemap* scr) {
   u8 eid = 0;
   
   // Erase all active entities in this screen
   while (eid < MAX_ENTITIES_SCR && m_scrEntities[eid]) {
      eraseEntity(m_entities + m_scrEntities[eid], scr);
      ++eid;
   }
   // And also the player (entity 0)
   eraseEntity(m_entities, scr);
}

//////////////////////////////////////////////////////////////////////////////
// Draws a entity if it has moved
//
void drawEntity(TEntity* e, void* pvmem) {
   pvmem = cpct_getScreenPtr(pvmem, (e->nx >> 2), (e->ny >> 1));
   cpct_drawSpriteMaskedAlignedTable(e->sprite, pvmem, e->w, e->h, cpct_transparentMaskTableM0_address);
}

//////////////////////////////////////////////////////////////////////////////
// Draws all entities on the present screen, plus the player
//
void drawAllEntities(void *pvmem) {
   u8 eid = 0;
   
   // Draw all active entities in this screen
   while (eid < MAX_ENTITIES_SCR && m_scrEntities[eid]) {
      drawEntity(m_entities + m_scrEntities[eid], pvmem);
      ++eid;
   }
   // And also the player (entity 0)
   drawEntity(m_entities, pvmem);
}

//////////////////////////////////////////////////////////////////////////////
// Create new game entity
//    Assumes there is space for new entities
//
TEntity* createGameEntity(u8 scr, u8 x, u8 y, u8 w, u8 h, u8* sprite) {
   TEntity*    e = (m_entities + m_nextFreeEnt);
   TAnimFrame* f;

   // Setup everything
   e->status = ES_alive;
   e->x      = e->nx = x;
   e->y      = e->ny = y;
   e->vx     = e->vy = 0;
   e->scr    = scr;
   e->sprite = sprite;
   e->w      = w;
   e->h      = h;

   m_numEnt++;
   locateNextFreeEntity();

   return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Updates an animation (controlling cycles and changing frames when required)
// Return Value:
//    Returns 0 when no event happened during update
//    Returns 1 when a new animation frame event has happened (animation changed)
//
u8 updateAnimation(TAnimation *anim) {
   u8 newframe = 0;

   // Update only if animation is not paused or finished
   if (anim->status != AS_pause && anim->status != AS_end) {

      // Update time and, If time has finished for this frame, get next
      if ( ! --anim->time ) {
         TAnimFrame* frame;

         // Next frame
         newframe = 1;
         frame = anim->frames[ ++anim->frame_id ];

         // If frame is not null, we have a new frame, else animation may have ended or may recycle
         if (frame) {
            // New frame values
            anim->time = frame->time;
         } else if ( anim->status == AS_cycle ) {
            // Recycle to first frame
            anim->frame_id = 0;
            anim->time     = anim->frames[0]->time;
         } else {
            // End animation (leaving last frame_id visible)
            --anim->frame_id;
            anim->status = AS_end;
         }
      }
   }

   // Report if a new frame has started
   return newframe;
}

//////
/// set a new animation or anim status
/*
   // If new animation, set it!
   if ( newAnim ) {
      anim->frames   = newAnim;    // Sets the new animation to the entity
      anim->frame_id = 0;          // First frame of the animation
      
      // Set time on non void animations
      if ( newAnim[0] )
         anim->time = newAnim[0]->time; // Animation is at its initial timestep

      newframe = 1; // We have changed animation, an that makes this a new frame
   }

   // If new animation status, set it!
   if ( newStatus )
      anim->status = newStatus;  // Set the initial status for the animation    
*/
