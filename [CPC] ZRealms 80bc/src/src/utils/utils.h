//
// Some utility functions
//

#ifndef UTILS_H
#define UTILS_H

// Random u8
extern u8   setRandomSeed_u8(u8 newseed) __z88dk_fastcall;
extern u8   getRandom_u8(u8 entropy_byte) __z88dk_fastcall;
extern void drawTile(void *memory, void* tile) __z88dk_callee;

#endif