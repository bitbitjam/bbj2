;;-----------------------------LICENSE NOTICE------------------------------------
;;  This file is part of the game: ZRealms 80 b.c.
;;  Copyright (C) 2015 ronaldo  / Cheesetea (@FranGallegoBR)
;;  Copyright (C) 2015 Predator / Cheesetea (@Hec_Linares)
;;
;;  This program is free software: you can redistribute it and/or modify
;;  it under the terms of the GNU General Public License as published by
;;  the Free Software Foundation, either version 3 of the License, or
;;  (at your option) any later version.
;;
;;  This program is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;  GNU General Public License for more details.
;;
;;  You should have received a copy of the GNU General Public License
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;------------------------------------------------------------------------------
 
.globl cpct_drawTileAligned2x4_f_asm

_drawTile::
   pop    hl       ;; 3
   pop    de       ;; 3
   ex    (sp), hl  ;; 6
   call  cpct_drawTileAligned2x4_f_asm ;;[5+59]
   ret             ;; 3